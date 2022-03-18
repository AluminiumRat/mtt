#include <mtt/utilities/static_cast_unique_ptr.h>

#include <Objects/FrameObject.h>

FrameObject::FrameObject( const QString& name,
                          bool canBeRenamed,
                          const mtt::UID& id) :
  HierarhicalObject(name, canBeRenamed, id)
{
}

void FrameObject::addChild(std::unique_ptr<HierarhicalObject> object)
{
  HierarhicalObject& objectRef = *object;
  addSubobject(std::move(object));
  emit childAdded(objectRef);
}

std::unique_ptr<HierarhicalObject> FrameObject::removeChild(
                                                  HierarhicalObject& object,
                                                  bool removeFromScene) noexcept
{
  std::unique_ptr<Object> removedObject = removeSubobject(object,
                                                          removeFromScene);
  std::unique_ptr<HierarhicalObject> removed3DObject =
            static_cast_unique_ptr<HierarhicalObject>(std::move(removedObject));
  if (removedObject != nullptr) emit childRemoved(*removed3DObject);
  return removed3DObject;
}

bool FrameObject::subobjectCanBeAdded(const Object& subobject) const noexcept
{
  const HierarhicalObject* modificator =
                            qobject_cast<const HierarhicalObject*>(&subobject);
  return modificator != nullptr;
}

std::unique_ptr<mtt::Object> FrameObject::tryAddSubobject(
                                            std::unique_ptr<mtt::Object> object)
{
  HierarhicalObject* modificator =
                                qobject_cast<HierarhicalObject*>(object.get());
  if(modificator == nullptr) return std::move(object);
  addChild(static_cast_unique_ptr<HierarhicalObject>(std::move(object)));
  return nullptr;
}

bool FrameObject::subobjectCanBeRemoved(
                                        const Object& subobject) const noexcept
{
  const HierarhicalObject* modificator =
                            qobject_cast<const HierarhicalObject*>(&subobject);
  return modificator != nullptr;
}

std::unique_ptr<mtt::Object> FrameObject::tryRemoveSubobject(
                                                  mtt::Object& subobject,
                                                  bool removeFromScene) noexcept
{
  HierarhicalObject* modificator = qobject_cast<HierarhicalObject*>(&subobject);
  if(modificator == nullptr) return nullptr;
  return removeChild(
                  static_cast<HierarhicalObject&>(subobject), removeFromScene);
}
