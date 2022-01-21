#pragma once

#include <mtt/application/Manipulator/AbstractManipulator.h>
#include <mtt/render/SceneGraph/AbstractNode.h>

namespace mtt
{
  class Manipulator3D : public AbstractManipulator
  {
  public:
    Manipulator3D() = default;
    Manipulator3D(const Manipulator3D&) = delete;
    Manipulator3D& operator = (const Manipulator3D&) = delete;
    virtual ~Manipulator3D() noexcept = default;

    virtual AbstractNode& node() noexcept = 0;
  };
}