#pragma once

#include <memory>

#include <mtt/render/SceneGraph/CombinedDrawableNode.h>
#include <mtt/render/SceneGraph/DrawableNode.h>

#include <Physics/PhysicsObject.h>

class PhysicsScene;
class RenderScene;

/// This is object of world. Provides interaction between a physical simulation
/// object and drawable object
class Object
{
public:
  Object(PhysicsScene& physycsScene, RenderScene& renderScene);
  Object(const Object&) = delete;
  Object& operator = (const Object&) = delete;
  ~Object() noexcept;

  inline mtt::DrawableNode* drawable() noexcept;
  inline const mtt::DrawableNode* drawable() const noexcept;
  void setDrawable(std::unique_ptr<mtt::DrawableNode> newDrawable);

  inline PhysicsObject* physicsObject() noexcept;
  inline const PhysicsObject* physicsObject() const noexcept;
  void setPhysicsObject(std::unique_ptr<PhysicsObject> newPhysicsObject);

  // Synchronize drawable and physics object
  void synchronize();

private:
  PhysicsScene& _physycsScene;
  RenderScene& _renderScene;

  std::unique_ptr<mtt::DrawableNode> _drawable;
  mtt::CombinedDrawableNode _rootNode;

  std::unique_ptr<PhysicsObject> _physicsObject;
};

inline mtt::DrawableNode* Object::drawable() noexcept
{
  return _drawable.get();
}

inline const mtt::DrawableNode* Object::drawable() const noexcept
{
  return _drawable.get();
}

inline PhysicsObject* Object::physicsObject() noexcept
{
  return _physicsObject.get();
}

inline const PhysicsObject* Object::physicsObject() const noexcept
{
  return _physicsObject.get();
}
