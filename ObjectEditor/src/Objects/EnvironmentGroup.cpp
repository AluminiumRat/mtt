#include <Objects/EnvironmentGroup.h>

EnvironmentGroup::EnvironmentGroup( const QString& name,
                                    bool canBeRenamed,
                                    const mtt::UID& theId) :
  SpecialGroup(name, canBeRenamed, theId)
{
  mtt::UID backgroundId(id().mixedUID(1096043266422623895ull));
  std::unique_ptr<BackgroundObject> background(
                                        new BackgroundObject( tr("Background"),
                                                              false,
                                                              backgroundId));
  _background = background.get();
  _background->setColor(glm::vec3(.3f, .5f, .7f));
  addSubobject(std::move(background));
}
