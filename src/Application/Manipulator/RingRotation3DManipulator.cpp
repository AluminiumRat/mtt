#include <optional>
#include <vector>

#include <glm/gtc/constants.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/gtx/transform.hpp>

#include <mtt/Application/Manipulator/RingRotation3DManipulator.h>
#include <mtt/Render/SceneGraph/CameraNode.h>
#include <mtt/Utilities/lineLineNearest.h>

using namespace mtt;

RingRotation3DManipulator::RingRotation3DManipulator(
                                  AutoscaleDrawableModificator::Mode scaleMode):
  Simple3DManipulator(scaleMode)
{
}

std::optional<glm::vec3> RingRotation3DManipulator::tryActivate(
                                            const TouchInfo& touchInfo) noexcept
{
  std::optional<glm::vec3> intersecPoint =
                                    Simple3DManipulator::tryActivate(touchInfo);
  if(!intersecPoint.has_value()) return std::nullopt;

  glm::vec3 planeOrigin = node().transformMatrix() * glm::vec4(0,0,0,1);
  _rotationAxis =
                glm::normalize(node().transformMatrix() * glm::vec4(0,0,1,0));

  _ringRadius = glm::length(intersecPoint.value() - planeOrigin);
  _startTangentOrigin = intersecPoint.value();

  _startTangentDirection = cross( _startTangentOrigin - planeOrigin,
                                  _rotationAxis);
  _startTangentDirection = glm::normalize(_startTangentDirection);

  return intersecPoint;
}

void RingRotation3DManipulator::processTouchMove(
                                            const TouchInfo& touchInfo) noexcept
{
  if(state() != ACTIVATED) return;
  if(_ringRadius == 0) return;

  glm::vec3 mouseDirection = touchInfo.camera.getDirection(
                                                  touchInfo.touchPositionNorm);
  std::optional<glm::vec3> touchPoint = lineLineNearest(
                                                    touchInfo.camera.eyePoint(),
                                                    mouseDirection,
                                                    _startTangentOrigin,
                                                    _startTangentDirection);
  if(touchPoint.has_value())
  {
    float shift = glm::dot( _startTangentOrigin - touchPoint.value(),
                            _startTangentDirection);
    float angle = shift / _ringRadius;
    processRotation(glm::rotate(angle, _rotationAxis));
  }
}
