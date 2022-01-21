#pragma once

#include <mtt/render/SceneGraph/Joint.h>

#include <Objects/Object3D.h>
#include <Render/AbstractObjectRenderObserver.h>

class Object3DRenderObserver : public AbstractObjectRenderObserver
{
  Q_OBJECT

public:
  Object3DRenderObserver(Object3D& object, EditorCommonData& commonData);
  Object3DRenderObserver(const Object3DRenderObserver&) = delete;
  Object3DRenderObserver& operator =
                                (const Object3DRenderObserver&) = delete;
  virtual ~Object3DRenderObserver() noexcept = default;

  inline Object3D& object() const noexcept;

protected:
  inline mtt::Joint& positionJoint() noexcept;
  inline const mtt::Joint& positionJoint() const noexcept;

  inline mtt::Joint& positionRotateJoint() noexcept;
  inline const mtt::Joint& positionRotateJoint() const noexcept;

  inline mtt::Joint& fullTransformJoint() noexcept;
  inline const mtt::Joint& fullTransformJoint() const noexcept;

private:
  void _setObjectTransform(const glm::mat4& transform) noexcept;

private:
  mtt::Joint _positionJoint;
  mtt::Joint _positionRotateJoint;
  mtt::Joint _fullTransformJoint;
};

inline Object3D& Object3DRenderObserver::object() const noexcept
{
  return static_cast<Object3D&>(AbstractObjectRenderObserver::object());
}

inline mtt::Joint& Object3DRenderObserver::positionJoint() noexcept
{
  return _positionJoint;
}

inline const mtt::Joint&
                          Object3DRenderObserver::positionJoint() const noexcept
{
  return _positionJoint;
}

inline mtt::Joint& Object3DRenderObserver::positionRotateJoint() noexcept
{
  return _positionRotateJoint;
}

inline const mtt::Joint&
                  Object3DRenderObserver::positionRotateJoint() const noexcept
{
  return _positionRotateJoint;
}

inline mtt::Joint& Object3DRenderObserver::fullTransformJoint() noexcept
{
  return _fullTransformJoint;
}

inline const mtt::Joint&
                    Object3DRenderObserver::fullTransformJoint() const noexcept
{
  return _fullTransformJoint;
}
