#include <algorithm>

#include <mtt/application/DrawModel/MasterDrawModel.h>

using namespace mtt;

MasterDrawModel::MasterDrawModel() :
  _locked(false)
{
}

std::optional<size_t> MasterDrawModel::findJoint(
                                            const QString& name) const noexcept
{
  for (size_t jointIndex = 0; jointIndex < _joints.size(); jointIndex++)
  {
    if(_joints[jointIndex].name == name) return jointIndex;
  }
  return std::nullopt;
}

void MasterDrawModel::addJoint( std::unique_ptr<Joint> joint,
                                TransformTable::Index boneIndex,
                                const QString& name)
{
  if(_locked) mtt::Abort("MasterDrawModel::addJoint: model is locked");
  if(joint == nullptr) Abort("MasterDrawModel::addJoint: joint is null");
  JointRecord newRecord;
  newRecord.node = std::move(joint);
  newRecord.boneIndex = boneIndex;
  newRecord.name = name;
  _joints.push_back(std::move(newRecord));
}

std::unique_ptr<Joint> MasterDrawModel::removeJoint(Joint& joint) noexcept
{
  if (_locked) mtt::Abort("MasterDrawModel::removeJoint: model is locked");
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

void MasterDrawModel::addMeshNode(std::unique_ptr<MeshControlNode> node)
{
  if (_locked) mtt::Abort("MasterDrawModel::addMeshNode: model is locked");
  if(node == nullptr) Abort("MasterDrawModel::addMeshNode: node is null");

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

std::unique_ptr<MeshControlNode> MasterDrawModel::removeMeshNode(
                                                MeshControlNode& node) noexcept
{
  if (_locked) mtt::Abort("MasterDrawModel::removeMeshNode: model is locked");
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
  removeNode(node);
  return tmp;
}

void MasterDrawModel::addAnimation(
                                  std::unique_ptr<DrawModelAnimation> animation,
                                  const QString& animationName)
{
  if (_locked) mtt::Abort("MasterDrawModel::addAnimation: model is locked");
  if (animation == nullptr) Abort("MasterDrawModel::addAnimation: animation is null");

  AnimationRecord newRecord;
  newRecord.animation = std::move(animation);
  newRecord.name = animationName;
  _animations.push_back(std::move(newRecord));
}

std::unique_ptr<DrawModelAnimation> MasterDrawModel::removeAnimation(
                                        DrawModelAnimation& animation) noexcept
{
  if (_locked) mtt::Abort("MasterDrawModel::removeAnimation: model is locked");
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

void MasterDrawModel::updateFromAnimation(const DrawModelAnimation& animation,
                                          Application::TimeType time)
{
  if (_locked) mtt::Abort("MasterDrawModel::updateFromAnimation: model is locked");
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

void MasterDrawModel::lock() noexcept
{
  _locked = true;
}
