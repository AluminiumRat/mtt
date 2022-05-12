#include <Objects/ObjectSaver.h>

void ObjectSaver::visitConstAnimationAction(const AnimationAction& object)
{
  PEVisitorT::visitConstAnimationAction(object);

  stream() << object.startTime();
  stream() << object.duration();
}


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
  stream() << object.firstAlbedo();
  stream() << object.secondAlbedo();
  stream() << object.opacityRange();
  stream() << object.emissionColor();
  stream() << object.emissionBrightness();
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

void ObjectSaver::visitConstGasEmissionAction(const GasEmissionAction& object)
{
  PEVisitorT::visitConstGasEmissionAction(object);

  stream() << object.emittedVolume();
  stream() << object.gasSourceRef().referencedId();
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

void ObjectSaver::visitConstHeatingAction(const HeatingAction& object)
{
  PEVisitorT::visitConstHeatingAction(object);
  stream() << object.emittedEnergy();
  stream() << object.heaterRef().referencedId();
}

void ObjectSaver::visitConstModificatorGroup(const ModificatorGroup& object)
{
  PEVisitorT::visitConstModificatorGroup(object);
  writeChilds(object);
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

  stream() << object.lodMppx();
  stream() << object.lodSmoothing();

  saveObjectData(object.fluid(), stream(), fileDirectory(), objectFactory());
}

void ObjectSaver::visitConstParticlesEmissionAction(
                                          const ParticlesEmissionAction& object)
{
  PEVisitorT::visitConstParticlesEmissionAction(object);

  stream() << uint32_t(object.particlesNumber());
  stream() << object.emitterRef().referencedId();
}

void ObjectSaver::visitConstRootObject(const RootObject& object)
{
  PEVisitorT::visitConstRootObject(object);

  saveObjectData( object.particleField(),
                  stream(),
                  fileDirectory(),
                  objectFactory());

  saveObjectData( object.modificatorsGroup(),
                  stream(),
                  fileDirectory(),
                  objectFactory());

  saveObjectData( object.animation(),
                  stream(),
                  fileDirectory(),
                  objectFactory());
}

void ObjectSaver::visitConstSizeControlObject(const SizeControlObject& object)
{
  PEVisitorT::visitConstSizeControlObject(object);

  stream() << object.startSize();
  stream() << object.endSize();
}

void ObjectSaver::visitConstVisibilityControlObject(
                                          const VisibilityControlObject& object)
{
  PEVisitorT::visitConstVisibilityControlObject(object);
  stream() << object.saturationRange();
}
