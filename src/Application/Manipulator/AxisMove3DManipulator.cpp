#include <optional>
#include <vector>

#include <mtt/Application/Manipulator/AxisMove3DManipulator.h>
#include <mtt/Render/SceneGraph/CameraNode.h>
#include <mtt/Utilities/lineLineNearest.h>

using namespace mtt;

AxisMove3DManipulator::AxisMove3DManipulator(
                                AutoscaleDrawableModificator::Mode scaleMode) :
  Simple3DManipulator(scaleMode)
{
}

std::optional<glm::vec3> AxisMove3DManipulator::tryActivate(
                                            const TouchInfo& touchInfo) noexcept
{
  std::optional<glm::vec3> intersecPoint =
                                    Simple3DManipulator::tryActivate(touchInfo);
  if(!intersecPoint.has_value()) return std::nullopt;

  _startAxisOrigin = node().transformMatrix() * glm::vec4(0,0,0,1);
  _startAxisDirection = node().transformMatrix() * glm::vec4(0, 0, 1, 0);
  _startAxisDirection = glm::normalize(_startAxisDirection);

  glm::vec3 mouseDirection =
                    touchInfo.camera.getDirection(touchInfo.touchPositionNorm);
  std::optional<glm::vec3> touchPoint = lineLineNearest(
                                                    touchInfo.camera.eyePoint(),
                                                    mouseDirection,
                                                    _startAxisOrigin,
                                                    _startAxisDirection);
  if(touchPoint.has_value()) _startTouchPoint = touchPoint.value();
  else _startTouchPoint = _startAxisOrigin;
  
  return intersecPoint;
}

void AxisMove3DManipulator::processTouchMove(
                                            const TouchInfo& touchInfo) noexcept
{
  if(state() != ACTIVATED) return;

  glm::vec3 mouseDirection = touchInfo.camera.getDirection(
                                                  touchInfo.touchPositionNorm);
  std::optional<glm::vec3> touchPoint = lineLineNearest(
                                                    touchInfo.camera.eyePoint(),
                                                    mouseDirection,
                                                    _startAxisOrigin,
                                                    _startAxisDirection);
  if(touchPoint.has_value())
  {
    processShift(_startTouchPoint, touchPoint.value());
  }
}
