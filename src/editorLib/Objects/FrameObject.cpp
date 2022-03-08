#include <mtt/editorLib/Objects/FrameObject.h>
#include <mtt/utilities/static_cast_unique_ptr.h>

using namespace mtt;

FrameObject::FrameObject( const QString& name,
                          bool canBeRenamed,
                          const UID& id) :
  ScalableObject(name, canBeRenamed, id)
{
}

void FrameObject::addChild(std::unique_ptr<Object3D> object)
{
  Object3D& objectRef = *object;
  addSubobject(std::move(object));
  emit childAdded(objectRef);
}

std::unique_ptr<Object3D> FrameObject::removeChild(
                                                  Object3D& object,
                                                  bool removeFromScene) noexcept
{
  std::unique_ptr<Object> removedObject = removeSubobject(object,
                                                          removeFromScene);
  std::unique_ptr<Object3D> removed3DObject =
                    static_cast_unique_ptr<Object3D>(std::move(removedObject));
  if (removedObject != nullptr) emit childRemoved(*removed3DObject);
  return removed3DObject;
}

bool FrameObject::subobjectCanBeAdded(const Object& subobject) const noexcept
{
  const Object3D* object3D = qobject_cast<const Object3D*>(&subobject);
  return object3D != nullptr;
}

std::unique_ptr<Object> FrameObject::tryAddSubobject(
                                                std::unique_ptr<Object> object)
{
  Object3D* object3D = qobject_cast<Object3D*>(object.get());
  if(object3D == nullptr) return std::move(object);
  addChild(static_cast_unique_ptr<Object3D>(std::move(object)));
  return nullptr;
}

bool FrameObject::subobjectCanBeRemoved(
                                        const Object& subobject) const noexcept
{
  const Object3D* object3D = qobject_cast<const Object3D*>(&subobject);
  return object3D != nullptr;
}

std::unique_ptr<Object> FrameObject::tryRemoveSubobject(
                                                  Object& subobject,
                                                  bool removeFromScene) noexcept
{
  Object3D* object3D = qobject_cast<Object3D*>(&subobject);
  if(object3D == nullptr) return nullptr;
  return removeChild(static_cast<Object3D&>(subobject), removeFromScene);
}
