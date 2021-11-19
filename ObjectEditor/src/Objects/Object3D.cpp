#include <glm/gtc/matrix_transform.hpp>

#include <mtt/Application/Scene/object_cast.h>

#include <Objects/Object3D.h>
#include <Objects/OEVisitor.h>

Object3D::Object3D(const mtt::UID& id) :
  DisplayedObject(id),
  _coordSystemTransform(1),
  _localTransform(1),
  _localToWorldTransform(1)
{
}

void Object3D::setCoordSystemTransform(const glm::mat4& newValue) noexcept
{
  _coordSystemTransform = newValue;
  _localToWorldTransform = _coordSystemTransform * _localTransform;
  _updateChildsTransform();
  emit coordSystemChanged(_coordSystemTransform);
  emit transformChanged(_localToWorldTransform);
}

void Object3D::setLocalTransform(const glm::mat4& newTransform) noexcept
{
  if(_localTransform == newTransform) return;
  _localTransform = newTransform;
  _localToWorldTransform = _coordSystemTransform * _localTransform;
  _updateChildsTransform();
  emit transformChanged(_localToWorldTransform);
}

void Object3D::_updateChildsTransform() noexcept
{
  for ( size_t childIndex = 0;
        childIndex < subobjectNumber();
        childIndex++)
  {
    mtt::Object& child = subobject(childIndex);
    Object3D* child3D = mtt::object_cast<Object3D, OEVisitor>(child);
    if (child3D != nullptr)
    {
      child3D->setCoordSystemTransform(localToWorldTransform());
    }
  }
}

void Object3D::onSubobjectAdded(Object& object) noexcept
{
  Object3D* child3D = mtt::object_cast<Object3D, OEVisitor>(object);
  if (child3D != nullptr)
  {
    child3D->setCoordSystemTransform(localToWorldTransform());
  }

  DisplayedObject::onSubobjectAdded(object);
}

void Object3D::onSubobjectRemoved(Object& object) noexcept
{
  Object3D* child3D = mtt::object_cast<Object3D, OEVisitor>(object);
  if (child3D != nullptr) child3D->setCoordSystemTransform(glm::mat4(1));

  DisplayedObject::onSubobjectRemoved(object);
}
