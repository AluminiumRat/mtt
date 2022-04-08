#include <Objects/ObjectLoader.h>

void ObjectLoader::visitBlockerObject(BlockerObject& object)
{
  PEVisitorT::visitBlockerObject(object);

  object.setShape(BlockerObject::Shape(stream().readUint8()));
  object.setSize(stream().readFloat());
}

void ObjectLoader::visitEmitterObject(EmitterObject& object)
{
  PEVisitorT::visitEmitterObject(object);

  object.setEnabled(stream().readBool());
  object.setTypeMask(stream().readUint32());
  object.fieldRef().setReferencedId(readUID());
  object.setIntensity(stream().readFloat());
  object.setSize(stream().readFloat());
  object.setShape(EmitterObject::Shape(stream().readUint8()));
  object.setDistribution(EmitterObject::Distribution(stream().readUint8()));
  object.setDirectionAngle(stream().readFloat());
  object.setSpeedRange(stream().readRange<float>());
  object.setSizeRange(stream().readRange<float>());
  object.setRotationRange(stream().readRange<float>());
  object.setRotationSpeedRange(stream().readRange<float>());
  object.setFirstColor(stream().readVec3());
  object.setSecondColor(stream().readVec3());
  object.setOpacityRange(stream().readRange<float>());
  object.setIgnoreBrightness(stream().readBool());
  object.setBrightnessRange(stream().readRange<float>());
  object.setTextureIndex(stream().readUint8());
  object.setTileIndex(stream().readUint8());
  object.setLifetimeRange(stream().readRange<mtt::TimeT>());
  object.setMassRange(stream().readRange<float>());
  object.setFrictionFactorRange(stream().readRange<float>());
}

void ObjectLoader::visitFrameObject(FrameObject& object)
{
  PEVisitorT::visitFrameObject(object);

  uint32_t childNumber = stream().readUint32();
  for (; childNumber != 0; childNumber--)
  {
    object.addChild(loadObject<HierarhicalObject>(true,
                                                  stream(),
                                                  fileDirectory(),
                                                  mixUIDValue(),
                                                  objectFactory()));
  }
}

void ObjectLoader::visitFluidModificator(FluidModificator& object)
{
  PEVisitorT::visitFluidModificator(object);

  object.setSize(stream().readFloat());
}

void ObjectLoader::visitFluidObject(FluidObject& object)
{
  PEVisitorT::visitFluidObject(object);

  object.setTypeMask(stream().readUint32());
  object.setCellSize(stream().readFloat());
  object.setWind(stream().readVec3());
  object.setSolverIterations(stream().readUint16());
}

void ObjectLoader::visitGasSource(GasSource& object)
{
  PEVisitorT::visitGasSource(object);

  object.setFlowRate(stream().readFloat());
  object.setTemperature(stream().readFloat());
}

void ObjectLoader::visitGravityModificator(GravityModificator& object)
{
  PEVisitorT::visitGravityModificator(object);

  object.setAcceleration(stream().readFloat());
}

void ObjectLoader::visitHeaterObject(HeaterObject& object)
{
  PEVisitorT::visitHeaterObject(object);

  object.setPower(stream().readFloat());
}

void ObjectLoader::visitModificatorObject(ModificatorObject& object)
{
  PEVisitorT::visitModificatorObject(object);

  object.setEnabled(stream().readBool());
  object.setTypeMask(stream().readUint32());
  object.fieldRef().setReferencedId(readUID());
}

void ObjectLoader::visitParticleAnimation(ParticleAnimation& object)
{
  PEVisitorT::visitParticleAnimation(object);

  object.setDuration(stream().readTime());
  object.fieldRef().setReferencedId(readUID());
}

void ObjectLoader::visitParticleField(ParticleField& object)
{
  PEVisitorT::visitParticleField(object);

  object.setSize(stream().readVec3());

  ParticleTextureDescriptions descriptions;
  uint8_t texturesCount = stream().readUint8();
  for (; texturesCount != 0; texturesCount--)
  {
    ParticleTextureDescription description;
    stream() >> description.filename;
    description.extent = stream().readUint8();
    descriptions.push_back(description);
  }
  object.setTextureDescriptions(descriptions);

  loadEmbeddedObject( object.fluid(),
                      stream(),
                      fileDirectory(),
                      mixUIDValue(),
                      objectFactory());
}

void ObjectLoader::visitSizeControlObject(SizeControlObject& object)
{
  PEVisitorT::visitSizeControlObject(object);

  object.setStartSize(stream().readFloat());
  object.setEndSize(stream().readFloat());
}

void ObjectLoader::visitVisibilityControlObject(VisibilityControlObject& object)
{
  PEVisitorT::visitVisibilityControlObject(object);
  object.setSaturationRange(stream().readRange<float>());
}
