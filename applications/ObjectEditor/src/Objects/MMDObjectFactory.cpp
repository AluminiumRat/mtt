#include <mtt/editorLib/Objects/AnimationObject.h>
#include <mtt/editorLib/Objects/PositionAnimator.h>
#include <mtt/editorLib/Objects/SkeletonObject.h>

#include <Objects/LODObject.h>
#include <Objects/MaterialObject.h>
#include <Objects/MeshObject.h>
#include <Objects/MMDObjectFactory.h>
#include <Objects/RootObject.h>

MMDObjectFactory::MMDObjectFactory()
{
  registerType<mtt::AnimationObject, animationIndex>();
  registerType<mtt::PositionAnimator, positionAnimatorIndex>();
  registerType<LODObject, lodIndex>();
  registerType<MaterialObject, materialIndex>();
  registerType<MeshObject, meshIndex>();
  registerType<mtt::SkeletonObject, skeletonIndex>();
  registerType<RootObject, rootObjectIndex>();
}
