#include <mtt/editorLib/Objects/AnimationTrack.h>

#include <Objects/EmitterObject.h>
#include <Objects/FrameObject.h>
#include <Objects/ParticleAnimation.h>
#include <Objects/ParticleField.h>
#include <Objects/PEEObjectFactory.h>

PEEObjectFactory::PEEObjectFactory()
{
  registerType<ParticleAnimation, animationIndex>();
  registerType<mtt::AnimationTrack, animationTrackIndex>();
  registerType<EmitterObject, emitterIndex>();
  registerType<FrameObject, frameIndex>();
  registerType<ParticleField, particleFieldIndex>();
}
