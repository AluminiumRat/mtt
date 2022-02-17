#include <glm/gtc/matrix_transform.hpp>

#include <mtt/editorLib/Objects/Object3D.h>

using namespace mtt;

Object3D::Object3D( const QString& name,
                    bool canBeRenamed,
                    const UID& id) :
  DisplayedObject(name, canBeRenamed, id),
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
    Object& child = subobject(childIndex);
    Object3D* child3D = qobject_cast<Object3D*>(&child);
    if (child3D != nullptr)
    {
      child3D->setCoordSystemTransform(localToWorldTransform());
    }
  }
}

void Object3D::onSubobjectAdded(Object& object) noexcept
{
  Object3D* child3D = qobject_cast<Object3D*>(&object);
  if (child3D != nullptr)
  {
    child3D->setCoordSystemTransform(localToWorldTransform());
  }

  DisplayedObject::onSubobjectAdded(object);
}

void Object3D::onSubobjectRemoved(Object& object) noexcept
{
  Object3D* child3D = qobject_cast<Object3D*>(&object);
  if (child3D != nullptr) child3D->setCoordSystemTransform(glm::mat4(1));

  DisplayedObject::onSubobjectRemoved(object);
}
