#include <mtt/application/DrawModel/SlaveDrawModel.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

SlaveDrawModel::SlaveDrawModel(std::shared_ptr<MasterDrawModel> masterModel) :
  _masterModel(masterModel),
  _transformTable(masterModel->transformTable())
{
  if(_masterModel == nullptr) Abort("SlaveDrawModel::SlaveDrawModel: master model is null.");
  masterModel->lock();

  NodeMap nodeMap;
  _copyJoints(nodeMap);
  _copyMeshes(nodeMap);
  _connectNodes(nodeMap);
  _switchBoneRefs(nodeMap);

  for(std::unique_ptr<SkinControlNode>& mesh : _meshes)
  {
    mesh->updateSkinFromBones();
  }
}

void SlaveDrawModel::_copyJoints(NodeMap& nodeMap)
{
  for ( size_t jointIndex = 0;
        jointIndex < _masterModel->jointsNumber();
        jointIndex++)
  {
    const Joint& originalJoint = _masterModel->getJoint(jointIndex);
    JointRecord newJoint;
    newJoint.boneIndex = _masterModel->boneIndex(jointIndex);
    newJoint.node.reset(new Joint());
    newJoint.node->setJointMatrix(originalJoint.jointMatrix());
    nodeMap[&originalJoint] = newJoint.node.get();
    _joints.push_back(std::move(newJoint));
  }
}

void SlaveDrawModel::_copyMeshes(NodeMap& nodeMap)
{
  for ( size_t meshIndex = 0;
        meshIndex < _masterModel->meshNodeNumber();
        meshIndex++)
  {
    MeshControlNode& originalNode = _masterModel->getMeshNode(meshIndex);
    std::unique_ptr<SkinControlNode> skinNode(new SkinControlNode());
    skinNode->setDrawable(&originalNode.mesh(),
                          originalNode.localBoundSphere());
    skinNode->setBoneRefs(originalNode.boneRefs());
    addNode(*skinNode);
    nodeMap[&originalNode] = skinNode.get();
    _meshes.push_back(std::move(skinNode));
  }
}

void SlaveDrawModel::_connectNodes(const NodeMap& nodeMap)
{
  for ( size_t jointIndex = 0;
        jointIndex < _masterModel->jointsNumber();
        jointIndex++)
  {
    const Joint& originalJoint = _masterModel->getJoint(jointIndex);
    Joint* slaveJoint = _joints[jointIndex].node.get();

    for ( size_t childIndex = 0;
          childIndex < originalJoint.childNumber();
          childIndex++)
    {
      const AbstractNode& originalChild = originalJoint.child(childIndex);
      NodeMap::const_iterator iChild = nodeMap.find(&originalChild);
      if(iChild != nodeMap.end()) slaveJoint->addChild(*iChild->second);
    }
  }
}

void SlaveDrawModel::_switchBoneRefs(const NodeMap& nodeMap)
{
  for (std::unique_ptr<SkinControlNode>& mesh : _meshes)
  {
    SkinControlNode::BoneRefs boneRefs = mesh->boneRefs();
    for (SkinControlNode::BoneRefData& refData : boneRefs)
    {
      NodeMap::const_iterator iChild = nodeMap.find(refData.joint);
      if (iChild != nodeMap.end()) refData.joint =
                                            static_cast<Joint*>(iChild->second);
    }
    mesh->setBoneRefs(boneRefs);
  }
}

void SlaveDrawModel::updateFromAnimation( const DrawModelAnimation& animation,
                                          TimeT time)
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

  for (std::unique_ptr<SkinControlNode>& mesh : _meshes)
  {
    mesh->updateSkinFromBones();
  }
}
