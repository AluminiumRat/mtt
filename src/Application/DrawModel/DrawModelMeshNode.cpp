#include <mtt/Application/DrawModel/DrawModelMeshNode.h>
#include <mtt/Render/SceneGraph/Joint.h>
#include <mtt/Utilities/Abort.h>

using namespace mtt;

DrawModelMeshNode::DrawModelMeshNode(LogicalDevice& device) :
  _mesh(device)
{
  _mesh.registerModificator(_lodFilter);
  setDrawable(&_mesh, Sphere());
}

void DrawModelMeshNode::setBoneRefs(const BoneRefs& newValue)
{
  if(!_boneRefs.empty()) _mesh.unregisterModificator(_boneMatricesSetter);
  _boneRefs.clear();

  if (!newValue.empty())
  {
    try
    {
      _boneMatricesSetter.matrices.resize(newValue.size());

      std::vector<glm::mat4> inverseMatrices;
      inverseMatrices.reserve(newValue.size());
      for (const BoneRefData& refData : newValue)
      {
        if(refData.joint == nullptr) Abort("DrawModelMeshNode::setBoneRefs:: null joint received.");
        inverseMatrices.push_back(refData.inverseBoneMatrix);
      }
      _mesh.extraData().setBoneInverseMatricesData(inverseMatrices);
      _mesh.registerModificator(_boneMatricesSetter);
      _boneRefs = newValue;
      updateSkinFromBones();
    }
    catch (...)
    {
      _mesh.extraData().removeBoneInverseMatricesData();
      _mesh.unregisterModificator(_boneMatricesSetter);
      _boneRefs.clear();
      throw;
    }
  }
}

void DrawModelMeshNode::updateSkinFromBones()
{
  glm::mat4 inverseOwnMatrix = glm::inverse(transformMatrix());

  for(size_t boneIndex = 0;
      boneIndex < _boneRefs.size();
      boneIndex++)
  {
    const Joint& joint = *_boneRefs[boneIndex].joint;
    glm::mat4 boneMatrix = joint.transformMatrix() * joint.jointMatrix();

    _boneMatricesSetter.matrices[boneIndex] = inverseOwnMatrix * boneMatrix;
  }
}
