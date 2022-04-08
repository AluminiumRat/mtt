#include <mtt/editorLib/Objects/AnimationTrack.h>

#include <Objects/Fluid/BlockerObject.h>
#include <Objects/Fluid/GasSource.h>
#include <Objects/Fluid/HeaterObject.h>
#include <Objects/EmitterObject.h>
#include <Objects/FrameObject.h>
#include <Objects/GravityModificator.h>
#include <Objects/ParticleAnimation.h>
#include <Objects/ParticleField.h>
#include <Objects/PEEObjectFactory.h>
#include <Objects/VisibilityControlObject.h>

PEEObjectFactory::PEEObjectFactory()
{
  registerType<ParticleAnimation, animationIndex>();
  registerType<mtt::AnimationTrack, animationTrackIndex>();
  registerType<EmitterObject, emitterIndex>();
  registerType<FrameObject, frameIndex>();
  registerType<ParticleField, particleFieldIndex>();
  registerType<VisibilityControlObject, visibilityControlIndex>();
  registerType<GravityModificator, gravityModificatorIndex>();
  registerType<BlockerObject, blockerObjectIndex>();
  registerType<GasSource, gasSourceIndex>();
  registerType<HeaterObject, heaterObjectIndex>();
}
