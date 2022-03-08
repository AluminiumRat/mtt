#include <mtt/utilities/static_cast_unique_ptr.h>

#include <Objects/FrameObject.h>

FrameObject::FrameObject( const QString& name,
                          bool canBeRenamed,
                          const mtt::UID& id) :
  ModificatorObject(name, canBeRenamed, id)
{
}

void FrameObject::addChild(std::unique_ptr<ModificatorObject> object)
{
  ModificatorObject& objectRef = *object;
  addSubobject(std::move(object));
  emit childAdded(objectRef);
}

std::unique_ptr<ModificatorObject> FrameObject::removeChild(
                                                  ModificatorObject& object,
                                                  bool removeFromScene) noexcept
{
  std::unique_ptr<Object> removedObject = removeSubobject(object,
                                                          removeFromScene);
  std::unique_ptr<ModificatorObject> removed3DObject =
            static_cast_unique_ptr<ModificatorObject>(std::move(removedObject));
  if (removedObject != nullptr) emit childRemoved(*removed3DObject);
  return removed3DObject;
}

bool FrameObject::subobjectCanBeAdded(const Object& subobject) const noexcept
{
  const ModificatorObject* modificator =
                            qobject_cast<const ModificatorObject*>(&subobject);
  return modificator != nullptr;
}

std::unique_ptr<mtt::Object> FrameObject::tryAddSubobject(
                                            std::unique_ptr<mtt::Object> object)
{
  ModificatorObject* modificator =
                                qobject_cast<ModificatorObject*>(object.get());
  if(modificator == nullptr) return std::move(object);
  addChild(static_cast_unique_ptr<ModificatorObject>(std::move(object)));
  return nullptr;
}

bool FrameObject::subobjectCanBeRemoved(
                                        const Object& subobject) const noexcept
{
  const ModificatorObject* modificator =
                            qobject_cast<const ModificatorObject*>(&subobject);
  return modificator != nullptr;
}

std::unique_ptr<mtt::Object> FrameObject::tryRemoveSubobject(
                                                  mtt::Object& subobject,
                                                  bool removeFromScene) noexcept
{
  ModificatorObject* modificator = qobject_cast<ModificatorObject*>(&subobject);
  if(modificator == nullptr) return nullptr;
  return removeChild(
                  static_cast<ModificatorObject&>(subobject), removeFromScene);
}
