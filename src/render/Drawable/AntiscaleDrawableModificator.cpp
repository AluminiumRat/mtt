#include <mtt/render/Drawable/AntiscaleDrawableModificator.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/utilities/ScopedSetter.h>

using namespace mtt;

AntiscaleDrawableModificator::AntiscaleDrawableModificator()
{
}

void AntiscaleDrawableModificator::draw(DrawPlanBuildInfo& buildInfo,
                                        DrawableModificator** next,
                                        size_t modifiactorsLeft,
                                        Drawable& drawable) const
{
  ValueRestorer<DrawMatrices> restoreMatrices(buildInfo.drawMatrices);

  glm::mat4 newLocalToViewMatrix = getCutedTransform(
                                      buildInfo.drawMatrices.localToViewMatrix);
  buildInfo.drawMatrices.localToViewMatrix = newLocalToViewMatrix;
  buildInfo.drawMatrices.normalsLocalToViewMatrix =
                            glm::transpose(glm::inverse(newLocalToViewMatrix));

  drawNext(buildInfo, next, modifiactorsLeft, drawable);
}

glm::mat4 AntiscaleDrawableModificator::getCutedTransform(
                                              const glm::mat4& origin) noexcept
{
  glm::vec3 xAxis = glm::normalize(origin[0]);
  glm::vec3 yAxis = glm::normalize(origin[1]);
  glm::vec3 zAxis = glm::normalize(glm::cross(xAxis, yAxis));
  yAxis = glm::normalize(glm::cross(zAxis, xAxis));

  glm::mat4 cuted;

  cuted[0] = glm::vec4(xAxis, 0.f);
  cuted[1] = glm::vec4(yAxis, 0.f);
  cuted[2] = glm::vec4(zAxis, 0.f);
  cuted[3] = origin[3];

  return cuted;
}
