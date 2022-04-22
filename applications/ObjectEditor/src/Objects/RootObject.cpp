#include <memory>
#include <stdexcept>

#include <mtt/utilities/Abort.h>

#include <Objects/RootObject.h>

RootObject::RootObject( const QString& name,
                        bool canBeRenamed,
                        const mtt::UID& theId) :
  Object(name, canBeRenamed, theId),
  _skeletonGroup(nullptr),
  _geometryGroup(nullptr),
  _materialsGroup(nullptr),
  _animationGroup(nullptr)
{
  mtt::UID skeletonId(id().mixedUID(3437741750129319833ull));
  std::unique_ptr<mtt::SkeletonGroup> skeletonGroup(
                                        new mtt::SkeletonGroup( tr("Skeleton"),
                                                                false,
                                                                skeletonId));
  _skeletonGroup = skeletonGroup.get();
  addSubobject(std::move(skeletonGroup));

  mtt::UID geometryId(id().mixedUID(672417181645139298ull));
  std::unique_ptr<GeometryGroup> geometryGroup(new GeometryGroup(
                                                                tr("Geometry"),
                                                                false,
                                                                geometryId));
  _geometryGroup = geometryGroup.get();
  addSubobject(std::move(geometryGroup));

  mtt::UID materialsId(id().mixedUID(17138545876999066826ull));
  std::unique_ptr<MaterialsGroup>
                              materialsGroup(new MaterialsGroup(tr("Materials"),
                                                                false,
                                                                materialsId));
  _materialsGroup = materialsGroup.get();
  addSubobject(std::move(materialsGroup));

  mtt::UID animationId(id().mixedUID(6931950026981487474ull));
  std::unique_ptr<mtt::AnimationGroup> animationGroup(
                                      new mtt::AnimationGroup(tr("Animations"),
                                                              false,
                                                              animationId));
  _animationGroup = animationGroup.get();
  addSubobject(std::move(animationGroup));
}

void RootObject::clear() noexcept
{
  while (_animationGroup->childsNumber() != 0)
  {
    _animationGroup->removeChild( _animationGroup->child(
                                          _animationGroup->childsNumber() - 1),
                                  true);
  }

  while (_geometryGroup->childsNumber() != 0)
  {
    _geometryGroup->removeChild(_geometryGroup->child(
                                            _geometryGroup->childsNumber() - 1),
                                true);
  }

  while (_materialsGroup->childsNumber() != 0)
  {
    _materialsGroup->removeChild( _materialsGroup->child(
                                          _materialsGroup->childsNumber() - 1),
                                  true);
  }

  while (_skeletonGroup->childsNumber() != 0)
  {
    _skeletonGroup->removeChild(_skeletonGroup->child(
                                            _skeletonGroup->childsNumber() - 1),
                                true);
  }
}
