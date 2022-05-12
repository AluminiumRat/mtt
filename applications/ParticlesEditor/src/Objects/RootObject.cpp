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
