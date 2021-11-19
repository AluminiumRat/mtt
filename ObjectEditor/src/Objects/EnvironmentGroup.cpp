#include <Objects/EnvironmentGroup.h>

EnvironmentGroup::EnvironmentGroup(const mtt::UID& theId) :
  SpecialGroup(theId)
{
  mtt::UID backgroundId(id().mixedUID(1096043266422623895ull));
  std::unique_ptr<BackgroundObject> background(
                                            new BackgroundObject(backgroundId));
  _background = background.get();
  _background->setObjectName(tr("Background"));
  _background->setColor(glm::vec3(.3f, .5f, .7f));
  addSubobject(std::move(background));
}
