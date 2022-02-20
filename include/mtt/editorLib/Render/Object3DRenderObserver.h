#pragma once

#include <mtt/editorLib/Objects/Object3D.h>
#include <mtt/editorLib/Render/AbstractObjectRenderObserver.h>
#include <mtt/render/SceneGraph/Joint.h>

namespace mtt
{
  class Object3DRenderObserver : public AbstractObjectRenderObserver
  {
    Q_OBJECT

  public:
    Object3DRenderObserver(Object3D& object, CommonEditData& commonData);
    Object3DRenderObserver(const Object3DRenderObserver&) = delete;
    Object3DRenderObserver& operator =
                                  (const Object3DRenderObserver&) = delete;
    virtual ~Object3DRenderObserver() noexcept = default;

    inline Object3D& object() const noexcept;

  protected:
    inline Joint& positionJoint() noexcept;
    inline const Joint& positionJoint() const noexcept;

    inline Joint& positionRotateJoint() noexcept;
    inline const Joint& positionRotateJoint() const noexcept;

    inline Joint& fullTransformJoint() noexcept;
    inline const Joint& fullTransformJoint() const noexcept;

  private:
    void _setObjectTransform(const glm::mat4& transform) noexcept;

  private:
    Joint _positionJoint;
    Joint _positionRotateJoint;
    Joint _fullTransformJoint;
  };

  inline Object3D& Object3DRenderObserver::object() const noexcept
  {
    return static_cast<Object3D&>(AbstractObjectRenderObserver::object());
  }

  inline Joint& Object3DRenderObserver::positionJoint() noexcept
  {
    return _positionJoint;
  }

  inline const Joint& Object3DRenderObserver::positionJoint() const noexcept
  {
    return _positionJoint;
  }

  inline Joint& Object3DRenderObserver::positionRotateJoint() noexcept
  {
    return _positionRotateJoint;
  }

  inline const Joint&
                    Object3DRenderObserver::positionRotateJoint() const noexcept
  {
    return _positionRotateJoint;
  }

  inline Joint& Object3DRenderObserver::fullTransformJoint() noexcept
  {
    return _fullTransformJoint;
  }

  inline const Joint&
                    Object3DRenderObserver::fullTransformJoint() const noexcept
  {
    return _fullTransformJoint;
  }
}