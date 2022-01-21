#pragma once

#include <optional>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <mtt/render/Drawable/Drawable.h>

namespace mtt
{
  class CameraNode;

  class AbstractManipulator
  {
  public:
    enum State
    {
      UNSELECTED,
      HIGHLITED,
      ACTIVATED
    };

    struct TouchInfo
    {
      glm::uvec2 touchPosition;
      glm::uvec2 frameSize;
      glm::vec2 touchPositionNorm;  // Touch position in normalized coordinates.
                                    // Each coordinate is in the range [-1; 1]
      const CameraNode& camera;
    };

  public:
    AbstractManipulator();
    AbstractManipulator(const AbstractManipulator&) = delete;
    AbstractManipulator& operator = (const AbstractManipulator&) = delete;
    virtual ~AbstractManipulator() noexcept = default;

    /// Returns 0 if the touch is not over the manipulator.
    /// Otherwise, returns distance to manipulator.
    virtual float checkTouchPosition(
                                const TouchInfo& touchInfo) const noexcept = 0;

    /// Returns intersection point if manipulator is selected by the touch.
    /// Returns nullopt otherwise
    virtual std::optional<glm::vec3> tryActivate(
                                      const TouchInfo& touchInfo) noexcept = 0;
    virtual void processTouchMove(const TouchInfo& touchInfo) noexcept = 0;
    virtual void release() noexcept;

    virtual Drawable& drawable() noexcept = 0;

    inline bool highlighted() const noexcept;
    void setHighlighted(bool newValue) noexcept;

    inline bool activated() const noexcept;

    inline State state() const noexcept;

  protected:
    virtual void processNewState(State newState) noexcept = 0;
    void setActivated(bool newValue) noexcept;

  private:
    void _updateState() noexcept;

  private:
    bool _highlighted;
    bool _activated;
    State _state;
  };

  inline bool AbstractManipulator::highlighted() const noexcept
  {
    return _highlighted;
  }

  inline bool AbstractManipulator::activated() const noexcept
  {
    return _activated;
  }

  inline AbstractManipulator::State AbstractManipulator::state() const noexcept
  {
    return _state;
  }
}