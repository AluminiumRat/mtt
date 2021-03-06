#include <mtt/application/DrawModel/MeshControlNode.h>
#include <mtt/render/SceneGraph/Joint.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

MeshControlNode::MeshControlNode(LogicalDevice& device) :
  _mesh(device)
{
  _mesh.addModificator(_lodFilter);
  setDrawable(&_mesh, Sphere());
}

void MeshControlNode::setBoneRefs(const BoneRefs& newValue)
{
  SkinControlNode::setBoneRefs(newValue);

  if (!newValue.empty())
  {
    try
    {
      std::vector<glm::mat4> inverseMatrices;
      inverseMatrices.reserve(newValue.size());
      for (const BoneRefData& refData : newValue)
      {
        if(refData.joint == nullptr) Abort("MeshControlNode::setBoneRefs:: null joint received.");
        inverseMatrices.push_back(refData.inverseBoneMatrix);
      }
      _mesh.extraData().setBoneInverseMatricesData(inverseMatrices);
    }
    catch (...)
    {
      _mesh.extraData().removeBoneInverseMatricesData();
      SkinControlNode::setBoneRefs(BoneRefs());
      throw;
    }
  }
}
