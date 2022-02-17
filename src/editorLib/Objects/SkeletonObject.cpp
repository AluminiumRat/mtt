#include <mtt/editorLib/Objects/SkeletonObject.h>
#include <mtt/utilities/static_cast_unique_ptr.h>

using namespace mtt;

SkeletonObject::SkeletonObject( const QString& name,
                                bool canBeRenamed,
                                const UID& id) :
  ScalableObject(name, canBeRenamed, id)
{
}

void SkeletonObject::addChild(std::unique_ptr<SkeletonObject> object)
{
  SkeletonObject& objectRef = *object;
  addSubobject(std::move(object));
  emit childAdded(objectRef);
}

std::unique_ptr<SkeletonObject> SkeletonObject::removeChild(
                                                  SkeletonObject& object,
                                                  bool removeFromScene) noexcept
{
  std::unique_ptr<Object> removedObject = removeSubobject(object,
                                                          removeFromScene);
  std::unique_ptr<SkeletonObject> removedSkeleton =
              static_cast_unique_ptr<SkeletonObject>(std::move(removedObject));
  if (removedObject != nullptr) emit childRemoved(*removedSkeleton);
  return removedSkeleton;
}

bool SkeletonObject::subobjectCanBeAdded(const Object& subobject) const noexcept
{
  const SkeletonObject* skeleton =
                                qobject_cast<const SkeletonObject*>(&subobject);
  return skeleton != nullptr;
}

std::unique_ptr<Object> SkeletonObject::tryAddSubobject(
                                                std::unique_ptr<Object> object)
{
  SkeletonObject* skeleton = qobject_cast<SkeletonObject*>(object.get());
  if(skeleton == nullptr) return std::move(object);
  addChild(static_cast_unique_ptr<SkeletonObject>(std::move(object)));
  return nullptr;
}

bool SkeletonObject::subobjectCanBeRemoved(
                                        const Object& subobject) const noexcept
{
  const SkeletonObject* skeleton =
                                qobject_cast<const SkeletonObject*>(&subobject);
  return skeleton != nullptr;
}

std::unique_ptr<Object> SkeletonObject::tryRemoveSubobject(
                                                  Object& subobject,
                                                  bool removeFromScene) noexcept
{
  SkeletonObject* skeleton = qobject_cast<SkeletonObject*>(&subobject);
  if(skeleton == nullptr) return nullptr;
  return removeChild(static_cast<SkeletonObject&>(subobject), removeFromScene);
}
