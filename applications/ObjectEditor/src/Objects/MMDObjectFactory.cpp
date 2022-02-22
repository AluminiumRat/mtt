#include <mtt/editorLib/Objects/AnimationObject.h>
#include <mtt/editorLib/Objects/AnimationTrack.h>
#include <mtt/editorLib/Objects/SkeletonObject.h>

#include <Objects/LODObject.h>
#include <Objects/MaterialObject.h>
#include <Objects/MeshObject.h>
#include <Objects/MMDObjectFactory.h>

MMDObjectFactory::MMDObjectFactory()
{
  registerType<mtt::AnimationObject, animationIndex>();
  registerType<mtt::AnimationTrack, animationTrackIndex>();
  registerType<LODObject, lodIndex>();
  registerType<MaterialObject, materialIndex>();
  registerType<MeshObject, meshIndex>();
  registerType<mtt::SkeletonObject, skeletonIndex>();
}
