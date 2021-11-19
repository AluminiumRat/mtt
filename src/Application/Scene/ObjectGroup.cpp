#include <mtt/Application/Scene/ObjectGroup.h>

using namespace mtt;

ObjectGroup::ObjectGroup(const UID& id) :
  Object(id)
{
}

void ObjectGroup::addChild(std::unique_ptr<Object> object)
{
  Object& objectRef = *object;
  addSubobject(std::move(object));
  emit childAdded(objectRef);
}

std::unique_ptr<Object> ObjectGroup::removeChild( Object& object,
                                                  bool removeFromScene) noexcept
{
  std::unique_ptr<Object> removedObject = removeSubobject(object,
                                                          removeFromScene);
  if(removedObject != nullptr) emit childRemoved(object);
  return removedObject;
}

bool ObjectGroup::subobjectCanBeAdded(const Object& subobject) const noexcept
{
  return true;
}

std::unique_ptr<Object> ObjectGroup::tryAddSubobject(
                                                std::unique_ptr<Object> object)
{
  addChild(std::move(object));
  return nullptr;
}

bool ObjectGroup::subobjectCanBeRemoved(const Object& subobject) const noexcept
{
  return true;
}

std::unique_ptr<Object> ObjectGroup::tryRemoveSubobject(
                                                  Object& subobject,
                                                  bool removeFromScene) noexcept
{
  return removeChild(subobject, removeFromScene);
}
