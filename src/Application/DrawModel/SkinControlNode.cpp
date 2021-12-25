#include <mtt/Application/DrawModel/SkinControlNode.h>
#include <mtt/Render/SceneGraph/Joint.h>
#include <mtt/Utilities/Abort.h>

using namespace mtt;

void SkinControlNode::setBoneRefs(const BoneRefs& newValue)
{
  if(!_boneRefs.empty()) unregisterModificator(_boneMatricesSetter);
  _boneRefs.clear();

  if (!newValue.empty())
  {
    try
    {
      _boneMatricesSetter.matrices.resize(newValue.size());
      registerModificator(_boneMatricesSetter);
      _boneRefs = newValue;
      updateSkinFromBones();
    }
    catch (...)
    {
      unregisterModificator(_boneMatricesSetter);
      _boneRefs.clear();
      throw;
    }
  }
}

void SkinControlNode::updateSkinFromBones()
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
