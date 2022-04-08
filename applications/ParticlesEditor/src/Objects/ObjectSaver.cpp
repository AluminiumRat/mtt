#include <Objects/ObjectSaver.h>

void ObjectSaver::visitConstBlockerObject(const BlockerObject& object)
{
  PEVisitorT::visitConstBlockerObject(object);

  stream() << (uint8_t)(object.shape());
  stream() << object.size();
}

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

void ObjectSaver::visitConstFluidModificator(const FluidModificator& object)
{
  PEVisitorT::visitConstFluidModificator(object);

  stream() << object.size();
}

void ObjectSaver::visitConstFluidObject(const FluidObject& object)
{
  PEVisitorT::visitConstFluidObject(object);

  stream() << object.typeMask();
  stream() << object.cellSize();
  stream() << object.wind();
  stream() << (uint16_t)(object.solverIterations());
}

void ObjectSaver::visitConstGasSource(const GasSource& object)
{
  PEVisitorT::visitConstGasSource(object);

  stream() << object.flowRate();
  stream() << object.temperature();
}

void ObjectSaver::visitConstGravityModificator(
                                              const GravityModificator& object)
{
  PEVisitorT::visitConstGravityModificator(object);

  stream() << object.acceleration();
}

void ObjectSaver::visitConstHeaterObject(const HeaterObject& object)
{
  PEVisitorT::visitConstHeaterObject(object);

  stream() << object.power();
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

  saveObjectData(object.fluid(), stream(), fileDirectory(), objectFactory());
}

void ObjectSaver::visitConstVisibilityControlObject(
                                          const VisibilityControlObject& object)
{
  PEVisitorT::visitConstVisibilityControlObject(object);
  stream() << object.saturationRange();
}
