#include <memory>
#include <stdexcept>

#include <mtt/utilities/Abort.h>

#include <Objects/RootObject.h>

RootObject::RootObject( const QString& name,
                        bool canBeRenamed,
                        const mtt::UID& theId) :
  Object(name, canBeRenamed, theId),
  _modificatorsGroup(nullptr),
  _animation(nullptr)
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
  _animation = animation.get();
  addSubobject(std::move(animation));

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

void RootObject::clear() noexcept
{
  try
  {
    while (_modificatorsGroup->childsNumber() != 0)
    {
      _modificatorsGroup->removeChild(_modificatorsGroup->child(0), true);
    }

    changeAnimation(
                  std::make_unique<ParticleAnimation>(tr("Animation"), false));
    changeParticleField(std::make_unique<ParticleField>(tr("Field"), false));
  }
  catch (std::exception& error)
  {
    mtt::Log() << error.what();
    mtt::Abort("RootObject::clear: unable to clear scene.");
  }
  catch (...)
  {
    mtt::Abort("RootObject::clear: unable to clear scene.");
  }
}
