#pragma once

#include <mtt/Application/Manipulator/Simple3DManipulator.h>

namespace mtt
{
  class UniformScale3DManipulator : public Simple3DManipulator
  {
  public:
    explicit UniformScale3DManipulator(
                                  AutoscaleDrawableModificator::Mode scaleMode);
    UniformScale3DManipulator(const UniformScale3DManipulator&) = delete;
    UniformScale3DManipulator& operator = (
                                    const UniformScale3DManipulator&) = delete;
    virtual ~UniformScale3DManipulator() noexcept = default;

    virtual std::optional<glm::vec3> tryActivate(
                                  const TouchInfo& touchInfo) noexcept override;
    virtual void processTouchMove(const TouchInfo& touchInfo) noexcept override;

  protected:
    /// Points in world coordinate system
    virtual void processScale(float scale) noexcept = 0;

  private:
    glm::vec3 _startPlaneOrigin;
    glm::vec3 _startPlaneNormal;
    float _startTouchDistance;
  };
}