#include <mtt/editorLib/Objects/CEObjectLoader.h>
#include <mtt/utilities/ScopedSetter.h>

using namespace mtt;

void CEObjectLoader::visitAmbientLightObject(AmbientLightObject& object)
{
  CEVisitorT::visitAmbientLightObject(object);
  object.setSaturationDistance(stream().readFloat());
  readCubemapData(object.ambientMap());
}

void CEObjectLoader::visitAnimationGroup(AnimationGroup& object)
{
  CEVisitorT::visitAnimationGroup(object);
  readChilds<AnimationGroup, AnimationObject>(object, true);
}

void CEObjectLoader::visitAnimationObject(AnimationObject& object)
{
  CEVisitorT::visitAnimationObject(object);
  uint32_t childNumber = stream().readUint32();
  for (; childNumber != 0; childNumber--)
  {
    object.addChild(loadObject<AnimationTrack>( true,
                                                stream(),
                                                fileDirectory(),
                                                mixUIDValue(),
                                                objectFactory()));
  }
}

void CEObjectLoader::visitAnimationTrack(AnimationTrack& object)
{
  CEVisitorT::visitAnimationTrack(object);
  object.setEnabled(stream().readBool());
}

void CEObjectLoader::visitBackgroundObject(BackgroundObject& object)
{
  CEVisitorT::visitBackgroundObject(object);
  object.setLightEnabled(stream().readBool());
  object.setLuminance(stream().readFloat());
  object.setColor(stream().readVec3());
  object.setDissolutionStartDistance(stream().readFloat());
  object.setDissolutionLength(stream().readFloat());
  readCubemapData(object.cubemap());
}

void CEObjectLoader::readCubemapData(CubemapObject& object)
{
  CubemapObject::Textures textures;
  for (QString& filename : textures)
  {
    filename = readFilename();
  }
  object.setTextures(textures);
}

void CEObjectLoader::visitCubemapObject(CubemapObject& object)
{
  CEVisitorT::visitCubemapObject(object);
  readCubemapData(object);
}

void CEObjectLoader::visitDirectLightObject(DirectLightObject& object)
{
  CEVisitorT::visitDirectLightObject(object);
  object.setCascadeSize(stream().readUint8());
  object.setShadowDistance(stream().readFloat());
}

void CEObjectLoader::visitDisplayedObject(DisplayedObject& object)
{
  CEVisitorT::visitDisplayedObject(object);
  object.setVisible(stream().readBool());
}

void CEObjectLoader::visitEnvironmentGroup(EnvironmentGroup& object)
{
  CEVisitorT::visitEnvironmentGroup(object);
  readChilds<EnvironmentGroup, EnvironmentObject>(object, true);
}

void CEObjectLoader::visitEnvironmentModel(EnvironmentModel& object)
{
  CEVisitorT::visitEnvironmentModel(object);
  object.setFilename(readFilename());
}

void CEObjectLoader::visitEnvironmentRootObject(EnvironmentRootObject& object)
{
  CEVisitorT::visitEnvironmentRootObject(object);

  loadEmbeddedObject( object.background(),
                      stream(),
                      fileDirectory(),
                      mixUIDValue(),
                      objectFactory());

  loadEmbeddedObject( object.objectsGroup(),
                      stream(),
                      fileDirectory(),
                      mixUIDValue(),
                      objectFactory());
}

void CEObjectLoader::visitFadingLightObject(FadingLightObject& object)
{
  CEVisitorT::visitFadingLightObject(object);
  object.setFadeType(clPipeline::LightingFade(stream().readUint8()));
}

void CEObjectLoader::visitLightObject(LightObject& object)
{
  CEVisitorT::visitLightObject(object);
  object.setEnabled(stream().readBool());
  object.setDistance(stream().readFloat());
  object.setColor(stream().readVec3());
  object.setBaseIlluminance(stream().readFloat());
}

void CEObjectLoader::visitLightWithShadowsObject(
                                                LightWithShadowsObject& object)
{
  CEVisitorT::visitLightWithShadowsObject(object);
  object.setShadowsEnabled(stream().readBool());
  object.setShadowmapSize(stream().readUint16());
  object.setShadowBlur(stream().readFloat());
}

void CEObjectLoader::visitMovableObject(MovableObject& object)
{
  CEVisitorT::visitMovableObject(object);
  object.setPosition(stream().readVec3());
}

void CEObjectLoader::visitPointLightObject(PointLightObject& object)
{
  CEVisitorT::visitPointLightObject(object);
  readCubemapData(object.filterCubemap());
}

void CEObjectLoader::visitPositionAnimator(PositionAnimator& object)
{
  CEVisitorT::visitPositionAnimator(object);

  uint16_t keypointsNumber = stream().readUint16();
  for (; keypointsNumber != 0; keypointsNumber--)
  {
    std::unique_ptr<PositionAnimator::PositionKeypoint> keypoint(
                                      new PositionAnimator::PositionKeypoint());
    readKeypoint(*keypoint);
    object.addPositionKeypoint(std::move(keypoint));
  }

  keypointsNumber = stream().readUint16();
  for (; keypointsNumber != 0; keypointsNumber--)
  {
    std::unique_ptr<PositionAnimator::RotationKeypoint> keypoint(
                                      new PositionAnimator::RotationKeypoint());
    readKeypoint(*keypoint);
    object.addRotationKeypoint(std::move(keypoint));
  }

  keypointsNumber = stream().readUint16();
  for (; keypointsNumber != 0; keypointsNumber--)
  {
    std::unique_ptr<PositionAnimator::ScaleKeypoint> keypoint(
                                        new PositionAnimator::ScaleKeypoint());
    readKeypoint(*keypoint);
    object.addScaleKeypoint(std::move(keypoint));
  }

  object.targetRef().setReferencedId(readUID());
}

void CEObjectLoader::visitRotatableObject(RotatableObject& object)
{
  CEVisitorT::visitRotatableObject(object);
  object.setRotation(stream().readQuat());
}

void CEObjectLoader::visitScalableObject(ScalableObject& object)
{
  CEVisitorT::visitScalableObject(object);
  object.setScale(stream().readVec3());
}

void CEObjectLoader::visitSkeletonGroup(SkeletonGroup& object)
{
  CEVisitorT::visitSkeletonGroup(object);
  readChilds<SkeletonGroup, SkeletonObject>(object, true);
}

void CEObjectLoader::visitSkeletonObject(SkeletonObject& object)
{
  CEVisitorT::visitSkeletonObject(object);
  uint32_t childsNumber = stream().readUint32();
  for (; childsNumber != 0; childsNumber--)
  {
    object.addChild(loadObject<SkeletonObject>( true,
                                                stream(),
                                                fileDirectory(),
                                                mixUIDValue(),
                                                objectFactory()));
  }
}

void CEObjectLoader::visitSpotLightObject(SpotLightObject& object)
{
  CEVisitorT::visitSpotLightObject(object);
  object.setAngle(stream().readFloat());
  object.setFilterImage(readFilename());
}
