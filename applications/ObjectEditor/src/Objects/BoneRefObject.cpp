#include <mtt/utilities/Abort.h>

#include <Objects/BoneRefObject.h>

BoneRefObject::BoneRefObject( const QString& name,
                              bool canBeRenamed,
                              const mtt::UID& id) :
  Object(name, canBeRenamed, id),
  _boneInverseMatrix(1),
  _link(*this)
{
}

void BoneRefObject::setBoneInverseMatrix(const glm::mat4& newValue)
{
  if(_boneInverseMatrix == newValue) return;
  _boneInverseMatrix = newValue;
  emit boneInverseMatrixChanged(_boneInverseMatrix);
}
