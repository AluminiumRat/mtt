#include <memory>
#include <stdexcept>

#include <mtt/utilities/Abort.h>

#include <Objects/RootObject.h>

RootObject::RootObject( const QString& name,
                        bool canBeRenamed,
                        const mtt::UID& theId) :
  Object(name, canBeRenamed, theId),
  _modificatorsGroup(nullptr),
  _animation(nullptr),
  _environment(nullptr)
{
  mtt::UID modificatorsId(id().mixedUID(6078948842405049854ull));
  std::unique_ptr<ModificatorGroup> modificatorsGroup(
                                      new ModificatorGroup( tr("Modificators"),
                                                            false,
                                                            modificatorsId));
  _modificatorsGroup = modificatorsGroup.get();
  addSubobject(std::move(modificatorsGroup));

  mtt::UID animationId(id().mixedUID(9391846700167947331ull));
  std::unique_ptr<ParticleAnimation> animation(
                                          new ParticleAnimation(tr("Animation"),
                                                                false,
                                                                animationId));
  mtt::TimeT defaultLength =
              std::chrono::duration_cast<mtt::TimeT>(std::chrono::seconds(30));
  animation->setTimeRange(mtt::Range<mtt::TimeT>(mtt::TimeT(0), defaultLength));
  _animation = animation.get();
  addSubobject(std::move(animation));

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
  _animation->fieldRef().set(_particleField);
}

void RootObject::changeAnimation(
                                std::unique_ptr<ParticleAnimation> newAnimation)
{
  if(newAnimation == nullptr) mtt::Abort("RootObject::changeAnimation: newAnimation is nullpt.");

  std::unique_ptr<mtt::Object> oldAnimation = removeSubobject(*_animation,
                                                              false);
  ParticleAnimation* newAnimationPtr = newAnimation.get();
  try
  {
    addSubobject(std::move(newAnimation));
  }
  catch (std::exception& error)
  {
    mtt::Log() << error.what();
    mtt::Abort("RootObject::changeAnimation: unable to change animation.");
  }
  catch (...)
  {
    mtt::Abort("RootObject::changeAnimation: unable to change animation.");
  }

  _animation = newAnimationPtr;
  emit animationChanged(*_animation);
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

void RootObject::changeParticleField( std::unique_ptr<ParticleField> newField)
{
  if(newField == nullptr) mtt::Abort("RootObject::changeParticleField: newField is nullpt.");

  std::unique_ptr<mtt::Object> oldField = removeSubobject(*_particleField,
                                                          false);
  ParticleField* newFieldPtr = newField.get();
  try
  {
    addSubobject(std::move(newField));
  }
  catch (std::exception& error)
  {
    mtt::Log() << error.what();
    mtt::Abort("RootObject::changeParticleField: unable to change particle field.");
  }
  catch (...)
  {
    mtt::Abort("RootObject::changeParticleField: unable to change particle field.");
  }

  _particleField = newFieldPtr;
  emit particleFieldChanged(*_particleField);
}