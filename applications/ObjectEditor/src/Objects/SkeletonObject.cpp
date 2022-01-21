#include <mtt/application/Scene/object_cast.h>
#include <mtt/utilities/static_cast_unique_ptr.h>

#include <Objects/OEVisitor.h>
#include <Objects/SkeletonObject.h>

SkeletonObject::SkeletonObject( const QString& name,
                                bool canBeRenamed,
                                const mtt::UID& id) :
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

bool SkeletonObject::subobjectCanBeAdded(
                                    const mtt::Object& subobject) const noexcept
{
  const SkeletonObject* skeleton =
                  mtt::object_cast<const SkeletonObject, OEVisitor>(subobject);
  return skeleton != nullptr;
}

std::unique_ptr<mtt::Object> SkeletonObject::tryAddSubobject(
                                            std::unique_ptr<mtt::Object> object)
{
  SkeletonObject* skeleton =
                      mtt::object_cast<SkeletonObject, OEVisitor>(object.get());
  if(skeleton == nullptr) return std::move(object);
  addChild(static_cast_unique_ptr<SkeletonObject>(std::move(object)));
  return nullptr;
}

bool SkeletonObject::subobjectCanBeRemoved(
                                    const mtt::Object& subobject) const noexcept
{
  const SkeletonObject* skeleton =
                  mtt::object_cast<const SkeletonObject, OEVisitor>(subobject);
  return skeleton != nullptr;
}

std::unique_ptr<mtt::Object> SkeletonObject::tryRemoveSubobject(
                                                  mtt::Object& subobject,
                                                  bool removeFromScene) noexcept
{
  SkeletonObject* skeleton =
                        mtt::object_cast<SkeletonObject, OEVisitor>(subobject);
  if(skeleton == nullptr) return nullptr;
  return removeChild(static_cast<SkeletonObject&>(subobject), removeFromScene);
}
