#include <mtt/editorLib/Objects/PositionAnimator.h>

#include <Objects/Fluid/BlockerObject.h>
#include <Objects/Fluid/GasSource.h>
#include <Objects/Fluid/HeaterObject.h>
#include <Objects/EmitterObject.h>
#include <Objects/FrameObject.h>
#include <Objects/GasEmissionAction.h>
#include <Objects/GravityModificator.h>
#include <Objects/HeatingAction.h>
#include <Objects/ModificatorGroup.h>
#include <Objects/ParticleAnimation.h>
#include <Objects/ParticleField.h>
#include <Objects/ParticlesEmissionAction.h>
#include <Objects/PEEObjectFactory.h>
#include <Objects/RootObject.h>
#include <Objects/SizeControlObject.h>
#include <Objects/VisibilityControlObject.h>

PEEObjectFactory::PEEObjectFactory()
{
  registerType<ParticleAnimation, animationIndex>();
  registerType<mtt::PositionAnimator, positionAnimatorIndex>();
  registerType<EmitterObject, emitterIndex>();
  registerType<FrameObject, frameIndex>();
  registerType<ParticleField, particleFieldIndex>();
  registerType<VisibilityControlObject, visibilityControlIndex>();
  registerType<GravityModificator, gravityModificatorIndex>();
  registerType<BlockerObject, blockerObjectIndex>();
  registerType<GasSource, gasSourceIndex>();
  registerType<HeaterObject, heaterObjectIndex>();
  registerType<SizeControlObject, sizeControlIndex>();
  registerType<ParticlesEmissionAction, particlesEmissionActionIndex>();
  registerType<GasEmissionAction, gasEmissionActionIndex>();
  registerType<HeatingAction, heatingActionIndex>();
  registerType<RootObject, rootObjectIndex>();
  registerType<ModificatorGroup, modificatorGroupIndex>();
}
