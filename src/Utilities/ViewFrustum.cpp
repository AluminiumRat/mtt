#include <glm/vec4.hpp>

#include <mtt/Utilities/ViewFrustum.h>

using namespace mtt;

ViewFrustum::ViewFrustum() noexcept
{
  setInverseViewProjectionMatrix(glm::mat4());
}

ViewFrustum::ViewFrustum(const glm::mat4& inverseViewProjectionMatrix) noexcept
{
  setInverseViewProjectionMatrix(inverseViewProjectionMatrix);
}

void ViewFrustum::setInverseViewProjectionMatrix(
                                              const glm::mat4& matrix) noexcept
{
  glm::vec4 nearLeftUp4 = matrix * glm::vec4(-1.f, -1.f, 1.f, 1.f);
  glm::vec4 nearLeftDown4 = matrix * glm::vec4(-1.f, 1.f, 1.f, 1.f);
  glm::vec4 nearRightUp4 = matrix * glm::vec4(1.f, -1.f, 1.f, 1.f);
  glm::vec4 nearRightDown4 = matrix * glm::vec4(1.f, 1.f, 1.f, 1.f);
  glm::vec4 farLeftUp4 = matrix * glm::vec4(-1.f, -1.f, 0.f, 1.f);
  glm::vec4 farLeftDown4 = matrix * glm::vec4(-1.f, 1.f, 0.f, 1.f);
  glm::vec4 farRightUp4 = matrix * glm::vec4(1.f, -1.f, 0.f, 1.f);
  glm::vec4 farRightDown4 = matrix * glm::vec4(1.f, 1.f, 0.f, 1.f);

  glm::vec3 nearLeftUp = nearLeftUp4 / nearLeftUp4.w;
  glm::vec3 nearLeftDown = nearLeftDown4 / nearLeftDown4.w;
  glm::vec3 nearRightUp = nearRightUp4 / nearRightUp4.w;
  glm::vec3 nearRightDown = nearRightDown4 / nearRightDown4.w;
  glm::vec3 farLeftUp = farLeftUp4 / farLeftUp4.w;
  glm::vec3 farLeftDown = farLeftDown4 / farLeftDown4.w;
  glm::vec3 farRightUp = farRightUp4 / farRightUp4.w;
  glm::vec3 farRightDown = farRightDown4 / farRightDown4.w;

  _faces[FACE_RIGHT].setPointNormal(
                            farRightUp,
                            glm::normalize(
                                      glm::cross( nearRightUp - farRightUp,
                                                  farRightDown - farRightUp)));
  _faces[FACE_LEFT].setPointNormal(
                            nearLeftDown,
                            glm::normalize(
                                    glm::cross( nearLeftUp - nearLeftDown,
                                                farLeftDown - nearLeftDown)));
  _faces[FACE_UP].setPointNormal(
                            farLeftUp,
                            glm::normalize(
                                    glm::cross( farLeftUp - farRightUp,
                                                nearRightUp - farRightUp)));

  _faces[FACE_DOWN].setPointNormal(
                            nearRightDown,
                            glm::normalize(
                                    glm::cross( farLeftDown - nearLeftDown,
                                                nearRightDown - nearLeftDown)));
  _faces[FACE_NEAR].setPointNormal(
                            nearLeftUp,
                            glm::normalize(
                                    glm::cross( nearRightDown - nearLeftDown,
                                                nearLeftUp - nearLeftDown)));
  _faces[FACE_FAR].setPointNormal(
                            farRightDown,
                            glm::normalize(
                                    glm::cross( farRightDown - farRightUp,
                                                farLeftUp - farRightUp)));
}
