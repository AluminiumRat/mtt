#include <mtt/editorLib/Objects/AnimationTrack.h>

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
  registerType<GravityModificator, gravityModificatorIndex>();
  registerType<ParticleField, particleFieldIndex>();
  registerType<VisibilityControlObject, visibilityControlIndex>();
}
