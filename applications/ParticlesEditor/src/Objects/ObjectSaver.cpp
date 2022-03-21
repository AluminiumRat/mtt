#include <Objects/ObjectSaver.h>

void ObjectSaver::visitConstEmitterObject(const EmitterObject& object)
{
  PEVisitorT::visitConstEmitterObject(object);

  stream() << object.enabled();
  stream() << object.typeMask();
  stream() << object.fieldRef().referencedId();
  stream() << object.intensity();
  stream() << object.size();
  stream() << uint8_t(object.shape());
  stream() << uint8_t(object.distribution());
  stream() << object.directionAngle();
  stream() << object.speedRange();
  stream() << object.sizeRange();
  stream() << object.rotationRange();
  stream() << object.rotationSpeedRange();
  stream() << object.firstColor();
  stream() << object.secondColor();
  stream() << object.opacityRange();
  stream() << object.ignoreBrightness();
  stream() << object.brightnessRange();
  stream() << uint8_t(object.textureIndex());
  stream() << uint8_t(object.tileIndex());
  stream() << object.lifetimeRange();
  stream() << object.massRange();
  stream() << object.frictionFactorRange();
}

void ObjectSaver::visitConstFrameObject(const FrameObject& object)
{
  PEVisitorT::visitConstFrameObject(object);

  uint32_t childNumber = uint32_t(object.childsNumber());
  stream() << childNumber;
  for (uint32_t childIndex = 0; childIndex < childNumber; childIndex++)
  {
    saveObject( object.child(childIndex),
                stream(),
                fileDirectory(),
                objectFactory());
  }
}

void ObjectSaver::visitConstGravityModificator(
                                              const GravityModificator& object)
{
  PEVisitorT::visitConstGravityModificator(object);

  stream() << object.acceleration();
}

void ObjectSaver::visitConstModificatorObject(const ModificatorObject& object)
{
  PEVisitorT::visitConstModificatorObject(object);

  stream() << object.enabled();
  stream() << object.typeMask();
  stream() << object.fieldRef().referencedId();
}

void ObjectSaver::visitConstParticleAnimation(const ParticleAnimation& object)
{
  PEVisitorT::visitConstParticleAnimation(object);

  stream() << object.duration();
  stream() << object.fieldRef().referencedId();
}

void ObjectSaver::visitConstParticleField(const ParticleField& object)
{
  PEVisitorT::visitConstParticleField(object);

  stream() << object.size();

  uint8_t texturesNumber = uint8_t(object.textureDescriptions().size());
  stream() << texturesNumber;

  for ( uint8_t textureIndex = 0;
        textureIndex < texturesNumber;
        textureIndex++)
  {
    const ParticleTextureDescription& description =
                                    object.textureDescriptions()[textureIndex];
    stream() << description.filename;
    stream() << uint8_t(description.extent);
  }
}

void ObjectSaver::visitConstVisibilityControlObject(
                                          const VisibilityControlObject& object)
{
  PEVisitorT::visitConstVisibilityControlObject(object);
  stream() << object.saturationRange();
}
