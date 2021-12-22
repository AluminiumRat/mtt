#include <algorithm>

#include <mtt/Application/DrawModel/DrawModel.h>
#include <mtt/Utilities/Abort.h>

using namespace mtt;

DrawModel::DrawModel()
{
}

void DrawModel::addJoint( std::unique_ptr<Joint> joint,
                          DrawModelTransformTable::Index boneIndex)
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
  if(iJoint == _joints.end()) Abort("DrawModel::removeJoint: joint is not found");

  std::unique_ptr<Joint> tmp = std::move(iJoint->node);
  _joints.erase(iJoint);
  return tmp;
}

void DrawModel::addMeshNode(std::unique_ptr<DrawModelMeshNode> node)
{
  if(node == nullptr) Abort("DrawModel::addMeshNode: node is null");

  DrawModelMeshNode& meshRef = *node;
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

std::unique_ptr<DrawModelMeshNode> DrawModel::removeMeshNode(
                                        const DrawModelMeshNode& node) noexcept
{
  Meshes::iterator iMesh = std::find_if(
                  _meshes.begin(),
                  _meshes.end(),
                  [&](const std::unique_ptr<DrawModelMeshNode>& record) -> bool
                  {
                    return record.get() == &node;
                  });
  if(iMesh == _meshes.end()) Abort("DrawModel::removeMeshNode: node is not found");

  std::unique_ptr<DrawModelMeshNode> tmp = std::move(*iMesh);
  _meshes.erase(iMesh);
  return tmp;
}
