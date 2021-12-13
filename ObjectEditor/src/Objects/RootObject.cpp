#include <memory>

#include <Objects/RootObject.h>

RootObject::RootObject( const QString& name,
                        bool canBeRenamed,
                        const mtt::UID& theId) :
  Object(name, canBeRenamed, theId),
  _skeletonGroup(nullptr),
  _geometryGroup(nullptr),
  _materialsGroup(nullptr),
  _animationGroup(nullptr),
  _environment(nullptr)
{
  mtt::UID skeletonId(id().mixedUID(3437741750129319833ull));
  std::unique_ptr<SkeletonGroup> skeletonGroup(new SkeletonGroup(
                                                                tr("Skeleton"),
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
  std::unique_ptr<AnimationGroup> animationGroup(
                                          new AnimationGroup( tr("Animations"),
                                                              false,
                                                              animationId));
  _animationGroup = animationGroup.get();
  addSubobject(std::move(animationGroup));

  mtt::UID backgroundId(id().mixedUID(1096043266422623895ull));
  std::unique_ptr<BackgroundObject> background(
                                        new BackgroundObject( tr(" Background"),
                                                              false,
                                                              backgroundId));
  _background = background.get();
  _background->setColor(glm::vec3(.3f, .5f, .7f));
  addSubobject(std::move(background));

  mtt::UID environmentId(id().mixedUID(5000232373494366922ull));
  std::unique_ptr<EnvironmentGroup>
              environmentGroup(new EnvironmentGroup(tr(" Environment objects"),
                                                    false,
                                                    environmentId));
  _environment = environmentGroup.get();
  addSubobject(std::move(environmentGroup));
}
