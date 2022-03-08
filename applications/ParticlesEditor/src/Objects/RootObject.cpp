#include <memory>
#include <stdexcept>

#include <mtt/utilities/Abort.h>

#include <Objects/RootObject.h>

RootObject::RootObject( const QString& name,
                        bool canBeRenamed,
                        const mtt::UID& theId) :
  Object(name, canBeRenamed, theId),
  _modificatorsGroup(nullptr),
  _animationGroup(nullptr),
  _environment(nullptr)
{
  mtt::UID modificatorsId(id().mixedUID(6078948842405049854ull));
  std::unique_ptr<mtt::Object3DGroup> modificatorsGroup(
                                    new mtt::Object3DGroup( tr("Modificators"),
                                                            false,
                                                            modificatorsId));
  _modificatorsGroup = modificatorsGroup.get();
  addSubobject(std::move(modificatorsGroup));

  mtt::UID animationId(id().mixedUID(9391846700167947331ull));
  std::unique_ptr<mtt::AnimationGroup> animationGroup(
                                      new mtt::AnimationGroup(tr("Animations"),
                                                              false,
                                                              animationId));
  _animationGroup = animationGroup.get();
  addSubobject(std::move(animationGroup));

  mtt::UID backgroundId(id().mixedUID(11173449662433983694ull));
  std::unique_ptr<mtt::BackgroundObject> background(
                                  new mtt::BackgroundObject(tr(" Background"),
                                                            false,
                                                            backgroundId));
  _background = background.get();
  _background->setColor(glm::vec3(.3f, .5f, .7f));
  addSubobject(std::move(background));

  mtt::UID environmentId(id().mixedUID(15315977137038106945ull));
  std::unique_ptr<mtt::EnvironmentGroup> environmentGroup(
                          new mtt::EnvironmentGroup(tr(" Environment objects"),
                                                    false,
                                                    environmentId));
  _environment = environmentGroup.get();
  addSubobject(std::move(environmentGroup));

  mtt::UID fieldId(id().mixedUID(11264425345735994719ull));
  std::unique_ptr<ParticleField> field(new ParticleField( tr("Field"),
                                                          false,
                                                          fieldId));
  _particleField = field.get();
  addSubobject(std::move(field));
}

void RootObject::changeBackground(
                          std::unique_ptr<mtt::BackgroundObject> newBackground)
{
  if(newBackground == nullptr) mtt::Abort("RootObject::changeBackground: newBackground is nullpt.");

  std::unique_ptr<mtt::Object> oldBackgraund = removeSubobject( *_background,
                                                                false);
  mtt::BackgroundObject* newBackgroundPtr = newBackground.get();
  try
  {
    addSubobject(std::move(newBackground));
  }
  catch (std::exception& error)
  {
    mtt::Log() << error.what();
    mtt::Abort("RootObject::changeBackground: unable to change background.");
  }
  catch (...)
  {
    mtt::Abort("RootObject::changeBackground: unable to change background.");
  }

  _background = newBackgroundPtr;
  emit backgroundChanged(*_background);
}
