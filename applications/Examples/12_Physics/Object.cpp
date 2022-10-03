#include <glm/gtx/transform.hpp>

#include <Physics/PhysicsScene.h>
#include <Object.h>
#include <RenderScene.h>

Object::Object(PhysicsScene& physycsScene, RenderScene& renderScene) :
  _physycsScene(physycsScene),
  _renderScene(renderScene)
{
  _renderScene.addCulledDrawable(_rootNode);
}

Object::~Object() noexcept
{
  _renderScene.removeCulledDrawable(_rootNode);
  if(_physicsObject != nullptr) _physycsScene.unregisterObject(*_physicsObject);
}

void Object::setDrawable(std::unique_ptr<mtt::DrawableNode> newDrawable)
{
  if(_drawable != nullptr) _rootNode.removeNode(*_drawable);
  _drawable = std::move(newDrawable);
  if(_drawable != nullptr) _rootNode.addNode(*_drawable);
}

void Object::setPhysicsObject(std::unique_ptr<PhysicsObject> newPhysicsObject)
{
  if(_physicsObject != nullptr) _physycsScene.unregisterObject(*_physicsObject);
  _physicsObject = std::move(newPhysicsObject);
  if (_physicsObject != nullptr) _physycsScene.registerObject(*_physicsObject);
  synchronize();
}

void Object::synchronize()
{
  if (_physicsObject != nullptr)
  {
    glm::vec3 newPosition(_physicsObject->position().x,
                          0.f,
                          _physicsObject->position().y);
    _rootNode.setTransformMatrix(
            glm::translate(newPosition) *
            glm::rotate(_physicsObject->rotation(), glm::vec3(0.f, -1.f, 0.f)));
  }
}
