#include <memory>

#include <Objects/RootObject.h>

RootObject::RootObject(const mtt::UID& theId) :
  Object(theId),
  _skeletonGroup(nullptr),
  _geometryGroup(nullptr)
{
  mtt::UID skeletonId(id().mixedUID(3437741750129319833ull));
  std::unique_ptr<SkeletonGroup> skeletonGroup(new SkeletonGroup(skeletonId));
  skeletonGroup->setObjectName(tr("Skeleton"));
  _skeletonGroup = skeletonGroup.get();
  addSubobject(std::move(skeletonGroup));

  mtt::UID geometryId(id().mixedUID(672417181645139298ull));
  std::unique_ptr<GeometryGroup> geometryGroup(new GeometryGroup(geometryId));
  geometryGroup->setObjectName(tr("Geometry"));
  _geometryGroup = geometryGroup.get();
  addSubobject(std::move(geometryGroup));

  mtt::UID animationId(id().mixedUID(6931950026981487474ull));
  std::unique_ptr<AnimationGroup> animationGroup(
                                              new AnimationGroup(animationId));
  animationGroup->setObjectName(tr("Animations"));
  _animationGroup = animationGroup.get();
  addSubobject(std::move(animationGroup));

  mtt::UID ambientId(id().mixedUID(5000232373494366922ull));
  std::unique_ptr<EnvironmentGroup>
                              environmentGroup(new EnvironmentGroup(ambientId));
  environmentGroup->setObjectName(tr(" Environment"));
  _environment = environmentGroup.get();
  addSubobject(std::move(environmentGroup));
}
