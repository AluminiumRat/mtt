#include <glm/gtc/matrix_transform.hpp>

#include <mtt/render/SceneGraph/CameraNode.h>

using namespace mtt;

CameraNode::CameraNode() noexcept :
  _viewMatrix(1),
  _projectionMatrix(1),
  _nearDistance(0),
  _farDistance(1)
{
  _updateFromPositionMatrix();
  _updateFromProjectionMatrix();
}

void CameraNode::setTransformMatrix(const glm::mat4& newValue) noexcept
{
  AbstractNode::setTransformMatrix(newValue);
  _updateFromPositionMatrix();
}

void CameraNode::_updateFromPositionMatrix() noexcept
{
  _eyePoint = transformMatrix() * glm::vec4(0, 0, 0, 1);
  _frontVector = transformMatrix() * glm::vec4(0, 0, -1, 0);
  _frontVector = glm::normalize(_frontVector);
  _upVector = transformMatrix() * glm::vec4(0, 1, 0, 0);
  _upVector = glm::normalize(_upVector);
  _rightVector = transformMatrix() * glm::vec4(1, 0, 0, 0);
  _rightVector = glm::normalize(_rightVector);
  _viewMatrix = glm::inverse(transformMatrix());
}

void CameraNode::setPerspectiveProjection(float fovY,
                                          float aspect,
                                          float zNear,
                                          float zFar) noexcept
{
  setProjectionMatrix(projectionCorrect *
                                  glm::perspective(fovY, aspect, zNear, zFar));
}

void CameraNode::setOrthoProjection(float left,
                                    float right,
                                    float bottom,
                                    float top,
                                    float zNear,
                                    float zFar) noexcept
{
  setProjectionMatrix(projectionCorrect *
                            glm::ortho(left, right, bottom, top, zNear, zFar));
}

void CameraNode::setProjectionMatrix(glm::mat4 newValue) noexcept
{
  _projectionMatrix = newValue;
  _updateFromProjectionMatrix();
}

void CameraNode::_updateFromProjectionMatrix() noexcept
{
  _inverseProjectionMatrix = glm::inverse(_projectionMatrix);

  glm::vec4 nearPoint = _inverseProjectionMatrix * glm::vec4(0, 0, 1, 1);
  _nearDistance = fabs(nearPoint.z / nearPoint.w);

  glm::vec4 farPoint = _inverseProjectionMatrix * glm::vec4(0, 0, 0, 1);
  _farDistance = fabs(farPoint.z / farPoint.w);

  _frustum.setInverseViewProjectionMatrix(_inverseProjectionMatrix);
}

DrawMatrices CameraNode::buildDrawMatrices() const noexcept
{
  return DrawMatrices { _viewMatrix,
                        _viewMatrix,
                        _projectionMatrix,
                        _inverseProjectionMatrix,
                        glm::inverse(_viewMatrix)};
}

ViewInfo CameraNode::buildViewInfo(const glm::uvec2 screenExtent) const noexcept
{
  return ViewInfo { glm::vec3(0, 0, 0),
                    glm::vec3(0, 0, -1),
                    glm::vec3(0, 1, 0),
                    glm::vec3(1, 0, 0),
                    buildScreenSizeInfo(screenExtent),
                    nearDistance(),
                    farDistance()};
}

void CameraNode::applyRootViewInfo(
                            ViewInfo& rootInfo,
                            const glm::mat4& localToParentView) const noexcept
{
  glm::mat4 toCameraView = _viewMatrix * glm::inverse(localToParentView);
  rootInfo.viewPosition = toCameraView * glm::vec4(rootInfo.viewPosition, 1);

  rootInfo.viewFront = toCameraView * glm::vec4(rootInfo.viewFront, 0);
  rootInfo.viewFront = glm::normalize(rootInfo.viewFront);

  rootInfo.viewUp = toCameraView * glm::vec4(rootInfo.viewUp, 0);
  rootInfo.viewUp = glm::normalize(rootInfo.viewUp);

  rootInfo.viewRight = toCameraView * glm::vec4(rootInfo.viewRight, 0);
  rootInfo.viewRight = glm::normalize(rootInfo.viewRight);
}

ScreenSizeInfo CameraNode::buildScreenSizeInfo(
                                  const glm::uvec2 screenExtent) const noexcept
{
  glm::vec4 nearLeftUp4 = _inverseProjectionMatrix * glm::vec4(-1.f, -1.f, 1.f, 1.f);
  glm::vec4 nearLeftDown4 = _inverseProjectionMatrix * glm::vec4(-1.f, 1.f, 1.f, 1.f);
  glm::vec4 nearRightDown4 = _inverseProjectionMatrix * glm::vec4(1.f, 1.f, 1.f, 1.f);
  glm::vec4 farLeftUp4 = _inverseProjectionMatrix * glm::vec4(-1.f, -1.f, 0.f, 1.f);
  glm::vec4 farLeftDown4 = _inverseProjectionMatrix * glm::vec4(-1.f, 1.f, 0.f, 1.f);
  glm::vec4 farRightDown4 = _inverseProjectionMatrix * glm::vec4(1.f, 1.f, 0.f, 1.f);
  glm::vec3 nearLeftUp = nearLeftUp4 / nearLeftUp4.w;
  glm::vec3 nearLeftDown = nearLeftDown4 / nearLeftDown4.w;
  glm::vec3 nearRightDown = nearRightDown4 / nearRightDown4.w;
  glm::vec3 farLeftUp = farLeftUp4 / farLeftUp4.w;
  glm::vec3 farLeftDown = farLeftDown4 / farLeftDown4.w;
  glm::vec3 farRightDown = farRightDown4 / farRightDown4.w;

  ScreenSizeInfo sizes;
  sizes.nearPlaneSize = glm::vec2(glm::length(nearRightDown - nearLeftDown),
                                  glm::length(nearLeftUp - nearLeftDown));
  glm::vec2 nearMppx = sizes.nearPlaneSize / glm::vec2(screenExtent);
  sizes.nearMppx = std::min(nearMppx.x, nearMppx.y);

  sizes.farPlaneSize = glm::vec2( glm::length(farRightDown - farLeftDown),
                                  glm::length(farLeftUp - farLeftDown));
  glm::vec2 farMppx = sizes.farPlaneSize / glm::vec2(screenExtent);
  sizes.farMppx = std::min(farMppx.x, farMppx.y);
  return sizes;
}

glm::vec3 CameraNode::getDirection(
                              const glm::vec2& screenCoordinates) const noexcept
{
  glm::vec4 projectedPoint(screenCoordinates, 0, 1);
  glm::vec4 unprojectedPoint = (_inverseProjectionMatrix * projectedPoint);
  unprojectedPoint /= unprojectedPoint.w;
  unprojectedPoint = transformMatrix() * unprojectedPoint;
  return glm::normalize(glm::vec3(unprojectedPoint) - _eyePoint);
}
