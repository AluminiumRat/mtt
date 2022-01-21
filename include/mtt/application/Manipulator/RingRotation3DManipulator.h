#pragma once

#include <mtt/application/Manipulator/Simple3DManipulator.h>

namespace mtt
{
  /// Translate along local z axis
  class RingRotation3DManipulator : public Simple3DManipulator
  {
  public:
    explicit RingRotation3DManipulator(
                                  AutoscaleDrawableModificator::Mode scaleMode);
    RingRotation3DManipulator(const RingRotation3DManipulator&) = delete;
    RingRotation3DManipulator& operator = (
                                    const RingRotation3DManipulator&) = delete;
    virtual ~RingRotation3DManipulator() noexcept = default;

    virtual std::optional<glm::vec3> tryActivate(
                                  const TouchInfo& touchInfo) noexcept override;
    virtual void processTouchMove(const TouchInfo& touchInfo) noexcept override;

  protected:
    /// rotation in world coordinate system
    virtual void processRotation(const glm::mat4& rotation) noexcept = 0;

  private:
    float _ringRadius;
    glm::vec3 _rotationAxis;
    glm::vec3 _startTangentOrigin;
    glm::vec3 _startTangentDirection;
  };
}