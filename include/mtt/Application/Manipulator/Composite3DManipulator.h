#pragma once

#include <vector>

#include <mtt/Application/Manipulator/Manipulator3D.h>
#include <mtt/Render/Drawable/DrawableList.h>
#include <mtt/Render/SceneGraph/GroupNode.h>

namespace mtt
{
  class Composite3DManipulator : public Manipulator3D
  {
  public:
    Composite3DManipulator() = default;
    Composite3DManipulator(const Composite3DManipulator&) = delete;
    Composite3DManipulator& operator = (const Composite3DManipulator&) = delete;
    virtual ~Composite3DManipulator() noexcept = default;

    virtual float checkTouchPosition(
                            const TouchInfo& touchInfo) const noexcept override;
    virtual std::optional<glm::vec3> tryActivate(
                                  const TouchInfo& touchInfo) noexcept override;
    virtual void processTouchMove(const TouchInfo& touchInfo) noexcept override;
    virtual void release() noexcept override;

    virtual AbstractNode& node() noexcept override;
    virtual Drawable& drawable() noexcept override;

  protected:
    using Submanipulators = std::vector<Manipulator3D*>;
    inline Submanipulators& submanipulators() noexcept;
    inline const Submanipulators& submanipulators() const noexcept;
    void registerSubmanipulator(Manipulator3D& manipulator,
                                AbstractNode& joint);
  protected:
    virtual void processNewState(State newState) noexcept;

  private:
    DrawableList _drawable;
    GroupNode _node;

    Submanipulators _submanipulators;
  };

  inline Composite3DManipulator::Submanipulators&
                                Composite3DManipulator::submanipulators() noexcept
  {
    return _submanipulators;
  }

  inline const Composite3DManipulator::Submanipulators&
                          Composite3DManipulator::submanipulators() const noexcept
  {
    return _submanipulators;
  }
}