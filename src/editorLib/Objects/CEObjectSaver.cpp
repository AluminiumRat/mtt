#include <mtt/editorLib/Objects/CEObjectSaver.h>

using namespace mtt;

void CEObjectSaver::writeCubemapData(const mtt::CubemapObject& object)
{
  mtt::CubemapObject::Textures textures = object.textures();
  for (const QString& filename : textures)
  {
    writeFilename(filename);
  }
}

void CEObjectSaver::visitConstAmbientLightObject(
                                          const mtt::AmbientLightObject& object)
{
  CEVisitorT::visitConstAmbientLightObject(object);
  stream() << object.saturationDistance();
  writeCubemapData(object.ambientMap());
}

void CEObjectSaver::visitConstAnimationGroup(const AnimationGroup& object)
{
  CEVisitorT::visitConstAnimationGroup(object);
  writeChilds(object);
}

void CEObjectSaver::visitConstAnimationObject(
                                            const mtt::AnimationObject& object)
{
  CEVisitorT::visitConstAnimationObject(object);
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
void CEObjectSaver::visitConstAnimationTrack(const mtt::AnimationTrack& object)
{
  CEVisitorT::visitConstAnimationTrack(object);
  stream() << object.enabled();
}

void CEObjectSaver::visitConstBackgroundObject(
                                            const mtt::BackgroundObject& object)
{
  CEVisitorT::visitConstBackgroundObject(object);
  stream() << object.lightEnabled();
  stream() << object.luminance();
  stream() << object.color();
  stream() << object.dissolutionStartDistance();
  stream() << object.dissolutionLength();
  writeCubemapData(object.cubemap());
}

void CEObjectSaver::visitConstCubemapObject(const CubemapObject& object)
{
  CEVisitorT::visitConstCubemapObject(object);
  writeCubemapData(object);
}

void CEObjectSaver::visitConstDirectLightObject(
                                          const mtt::DirectLightObject& object)
{
  CEVisitorT::visitConstDirectLightObject(object);
  stream() << (uint8_t)object.cascadeSize();
  stream() << object.shadowDistance();
}

void CEObjectSaver::visitConstDisplayedObject(
                                            const mtt::DisplayedObject& object)
{
  CEVisitorT::visitConstDisplayedObject(object);
  stream() << object.visible();
}

void CEObjectSaver::visitConstEnvironmentGroup(const EnvironmentGroup& object)
{
  CEVisitorT::visitConstEnvironmentGroup(object);
  writeChilds(object);
}

void CEObjectSaver::visitConstEnvironmentModel(
                                            const mtt::EnvironmentModel& object)
{
  CEVisitorT::visitConstEnvironmentModel(object);
  writeFilename(object.filename());
}

void CEObjectSaver::visitConstEnvironmentRootObject(
                                            const EnvironmentRootObject& object)
{
  CEVisitorT::visitConstEnvironmentRootObject(object);

  saveObjectData( object.background(),
                  stream(),
                  fileDirectory(),
                  objectFactory());

  saveObjectData( object.objectsGroup(),
                  stream(),
                  fileDirectory(),
                  objectFactory());
}

void CEObjectSaver::visitConstFadingLightObject(const FadingLightObject& object)
{
  CEVisitorT::visitConstFadingLightObject(object);
  stream() << (uint8_t)object.fadeType();
}

void CEObjectSaver::visitConstLightObject(const mtt::LightObject& object)
{
  CEVisitorT::visitConstLightObject(object);
  stream() << object.enabled();
  stream() << object.distance();
  stream() << object.color();
  stream() << object.baseIlluminance();
}

void CEObjectSaver::visitConstLightWithShadowsObject(
                                          const LightWithShadowsObject& object)
{
  CEVisitorT::visitConstLightWithShadowsObject(object);
  stream() << object.shadowsEnabled();
  stream() << (uint16_t)object.shadowmapSize();
  stream() << object.shadowBlur();
}

void CEObjectSaver::visitConstMovableObject(const mtt::MovableObject& object)
{
  CEVisitorT::visitConstMovableObject(object);
  stream() << object.position();
}

void CEObjectSaver::visitConstPointLightObject(const PointLightObject& object)
{
  CEVisitorT::visitConstPointLightObject(object);
  writeCubemapData(object.filterCubemap());
}

void CEObjectSaver::visitConstPositionAnimator(const PositionAnimator& object)
{
  CEVisitorT::visitConstPositionAnimator(object);

  stream() <<(uint16_t)object.positionKeypointNumber();
  for ( size_t keypointIndex = 0;
        keypointIndex < object.positionKeypointNumber();
        keypointIndex++)
  {
    writeKeypoint(object.positionKeypoint(keypointIndex));
  }

  stream() <<(uint16_t)object.rotationKeypointNumber();
  for ( size_t keypointIndex = 0;
        keypointIndex < object.rotationKeypointNumber();
        keypointIndex++)
  {
    writeKeypoint(object.rotationKeypoint(keypointIndex));
  }

  stream() <<(uint16_t)object.scaleKeypointNumber();
  for ( size_t keypointIndex = 0;
        keypointIndex < object.scaleKeypointNumber();
        keypointIndex++)
  {
    writeKeypoint(object.scaleKeypoint(keypointIndex));
  }
  
  stream() << object.targetRef().referencedId();
}

void CEObjectSaver::visitConstRotatableObject(
                                            const mtt::RotatableObject& object)
{
  CEVisitorT::visitConstRotatableObject(object);
  stream() << object.rotation();
}

void CEObjectSaver::visitConstScalableObject(
                                              const mtt::ScalableObject& object)
{
  CEVisitorT::visitConstScalableObject(object);
  stream() << object.scale();
}

void CEObjectSaver::visitConstSkeletonGroup(const SkeletonGroup& object)
{
  CEVisitorT::visitConstSkeletonGroup(object);
  writeChilds(object);
}

void CEObjectSaver::visitConstSkeletonObject(const mtt::SkeletonObject& object)
{
  CEVisitorT::visitConstSkeletonObject(object);
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

void CEObjectSaver::visitConstSpotLightObject(const SpotLightObject& object)
{
  CEVisitorT::visitConstSpotLightObject(object);
  stream() << object.angle();
  writeFilename(object.filterImage());
}
