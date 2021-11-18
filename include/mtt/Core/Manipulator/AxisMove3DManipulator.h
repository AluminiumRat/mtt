#pragma once

#include <mtt/Core/Manipulator/Simple3DManipulator.h>

namespace mtt
{
  /// Translate along local z axis
  class AxisMove3DManipulator : public Simple3DManipulator
  {
  public:
    explicit AxisMove3DManipulator(
                                  AutoscaleDrawableModificator::Mode scaleMode);
    AxisMove3DManipulator(const AxisMove3DManipulator&) = delete;
    AxisMove3DManipulator& operator = (const AxisMove3DManipulator&) = delete;
    virtual ~AxisMove3DManipulator() noexcept = default;

    virtual std::optional<glm::vec3> tryActivate(
                                  const TouchInfo& touchInfo) noexcept override;
    virtual void processTouchMove(const TouchInfo& touchInfo) noexcept override;

  protected:
    /// Points in world coordinate system
    virtual void processShift(const glm::vec3& startTouchPoint,
                              const glm::vec3& shiftedPoint) noexcept = 0;

  private:
    glm::vec3 _startAxisOrigin;
    glm::vec3 _startAxisDirection;
    glm::vec3 _startTouchPoint;
  };
}