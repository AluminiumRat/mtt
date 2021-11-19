#pragma once

#include <mtt/Application/Manipulator/Simple3DManipulator.h>

namespace mtt
{
  /// Translate along local xy plane
  class PlaneMove3DManipulator : public Simple3DManipulator
  {
  public:
    explicit PlaneMove3DManipulator(
                                  AutoscaleDrawableModificator::Mode scaleMode);
    PlaneMove3DManipulator(const PlaneMove3DManipulator&) = delete;
    PlaneMove3DManipulator& operator = (const PlaneMove3DManipulator&) = delete;
    virtual ~PlaneMove3DManipulator() noexcept = default;

    virtual std::optional<glm::vec3> tryActivate(
                                  const TouchInfo& touchInfo) noexcept override;
    virtual void processTouchMove(const TouchInfo& touchInfo) noexcept override;

  protected:
    /// Points in world coordinate system
    virtual void processShift(const glm::vec3& startTouchPoint,
                              const glm::vec3& shiftedPoint) noexcept = 0;

  private:
    glm::vec3 _startPlaneOrigin;
    glm::vec3 _startPlaneNormal;
    glm::vec3 _startTouchPoint;
  };
}