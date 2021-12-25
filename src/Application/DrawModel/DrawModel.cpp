#include <algorithm>

#include <mtt/Application/DrawModel/DrawModel.h>
#include <mtt/Utilities/Abort.h>

using namespace mtt;

DrawModel::DrawModel()
{
}

void DrawModel::addJoint( std::unique_ptr<Joint> joint,
                          TransformTable::Index boneIndex)
{
  if(joint == nullptr) Abort("DrawModel::addJoint: joint is null");
  JointRecord newRecord;
  newRecord.node = std::move(joint);
  newRecord.boneIndex = boneIndex;
  _joints.push_back(std::move(newRecord));
}

std::unique_ptr<Joint> DrawModel::removeJoint(const Joint& joint) noexcept
{
  Joints::iterator iJoint = std::find_if( _joints.begin(),
                                          _joints.end(),
                                          [&](const JointRecord& record) -> bool
                                          {
                                            return record.node.get() == &joint;
                                          });
  if(iJoint == _joints.end()) return nullptr;

  std::unique_ptr<Joint> tmp = std::move(iJoint->node);
  _joints.erase(iJoint);
  return tmp;
}

void DrawModel::addMeshNode(std::unique_ptr<MeshControlNode> node)
{
  if(node == nullptr) Abort("DrawModel::addMeshNode: node is null");

  MeshControlNode& meshRef = *node;
  _meshes.push_back(std::move(node));

  try
  {
    addNode(meshRef);
  }
  catch (...)
  {
    _meshes.pop_back();
    throw;
  }
}

std::unique_ptr<MeshControlNode> DrawModel::removeMeshNode(
                                    const MeshControlNode& node) noexcept
{
  Meshes::iterator iMesh = std::find_if(
                    _meshes.begin(),
                    _meshes.end(),
                    [&](const std::unique_ptr<MeshControlNode>& record) -> bool
                    {
                      return record.get() == &node;
                    });
  if(iMesh == _meshes.end()) return nullptr;

  std::unique_ptr<MeshControlNode> tmp = std::move(*iMesh);
  _meshes.erase(iMesh);
  return tmp;
}

void DrawModel::addAnimation( std::unique_ptr<DrawModelAnimation> animation,
                              const QString& animationName)
{
  if (animation == nullptr) Abort("DrawModel::addAnimation: animation is null");

  AnimationRecord newRecord;
  newRecord.animation = std::move(animation);
  newRecord.name = animationName;
  _animations.push_back(std::move(newRecord));
}

std::unique_ptr<DrawModelAnimation> DrawModel::removeAnimation(
                                  const DrawModelAnimation& animation) noexcept
{
  Animations::iterator iAnimation =
                    std::find_if( _animations.begin(),
                                  _animations.end(),
                                  [&](const AnimationRecord& record) -> bool
                                  {
                                    return record.animation.get() == &animation;
                                  });
  if(iAnimation == _animations.end()) return nullptr;

  std::unique_ptr<DrawModelAnimation> tmp = std::move(iAnimation->animation);
  _animations.erase(iAnimation);
  return tmp;
}

void DrawModel::updateFromAnimation(const DrawModelAnimation& animation,
                                    Application::TimeType time)
{
  animation.updateTransform(_transformTable, time);

  for (JointRecord& jointRecord : _joints)
  {
    TransformTable::Index boneIndex = jointRecord.boneIndex;
    if(boneIndex != TransformTable::notIndex)
    {
      jointRecord.node->setJointMatrix(_transformTable.getTransform(boneIndex));
    }
  }

  for (std::unique_ptr<MeshControlNode>& mesh : _meshes)
  {
    mesh->updateSkinFromBones();
  }
}
