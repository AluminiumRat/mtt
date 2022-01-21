#include <glm/gtx/intersect.hpp>

#include <mtt/application/Manipulator/UniformScale3DManipulator.h>
#include <mtt/render/SceneGraph/CameraNode.h>

using namespace mtt;

UniformScale3DManipulator::UniformScale3DManipulator(
                                AutoscaleDrawableModificator::Mode scaleMode) :
  Simple3DManipulator(scaleMode),
  _startTouchDistance(1.f)
{
}

static std::optional<glm::vec3> intesect( glm::vec3 touchOrigin,
                                          glm::vec3 touchDirection,
                                          glm::vec3 planeOrigin,
                                          glm::vec3 planeNormal) noexcept
{
  float distance;
  if(!glm::intersectRayPlane( touchOrigin,
                              touchDirection,
                              planeOrigin,
                              planeNormal,
                              distance)) return std::nullopt;
  return touchOrigin + touchDirection * distance;
}

std::optional<glm::vec3> UniformScale3DManipulator::tryActivate(
                                            const TouchInfo& touchInfo) noexcept
{
  std::optional<glm::vec3> intersecPoint =
                                    Simple3DManipulator::tryActivate(touchInfo);
  if(!intersecPoint.has_value()) return std::nullopt;

  _startPlaneOrigin = node().transformMatrix() * glm::vec4(0,0,0,1);
  _startPlaneNormal =
              glm::normalize(node().transformMatrix() * glm::vec4(0, 0, 1, 0));
  _startPlaneNormal = glm::normalize(_startPlaneNormal);

  glm::vec3 mouseDirection = touchInfo.camera.getDirection(
                                                  touchInfo.touchPositionNorm);
  std::optional<glm::vec3> touchPoint = intesect( touchInfo.camera.eyePoint(),
                                                  mouseDirection,
                                                  _startPlaneOrigin,
                                                  _startPlaneNormal);
  if(touchPoint.has_value())
  {
    _startTouchDistance = glm::length(touchPoint.value() - _startPlaneOrigin);
    if(_startTouchDistance == 0) _startTouchDistance = 1.f;
  }
  else _startTouchDistance = 1.f;
  
  return intersecPoint;
}

void UniformScale3DManipulator::processTouchMove(
                                            const TouchInfo& touchInfo) noexcept
{
  if(state() != ACTIVATED) return;

  glm::vec3 mouseDirection = touchInfo.camera.getDirection(
                                                  touchInfo.touchPositionNorm);
  std::optional<glm::vec3> touchPoint = intesect( touchInfo.camera.eyePoint(),
                                                  mouseDirection,
                                                  _startPlaneOrigin,
                                                  _startPlaneNormal);
  if(touchPoint.has_value())
  {
    float newTouchDistance =
                            glm::length(touchPoint.value() - _startPlaneOrigin);
    processScale(newTouchDistance / _startTouchDistance);
  }
}
