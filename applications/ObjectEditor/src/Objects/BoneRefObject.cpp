#include <mtt/Utilities/Abort.h>

#include <Objects/BoneRefObject.h>

BoneRefObject::BoneRefObject( const QString& name,
                              bool canBeRenamed,
                              const mtt::UID& id) :
  Object(name, canBeRenamed, id),
  _boneInverseMatrix(1),
  _link(*this)
{
}

void BoneRefObject::setBone(SkeletonObject* bone)
{
  if(bone == nullptr) setBoneId(mtt::UID());
  else setBoneId(bone->id());
}

void BoneRefObject::setBoneId(const mtt::UID& id)
{
  try
  {
    _link.setReferencedId(id);
  }
  catch (...)
  {
    try
    {
      _link.setReferencedId(mtt::UID());
    }
    catch (...)
    {
      mtt::Abort("BoneRefObject::setBoneId: unable to emplace skeleton link");
    }
    throw;
  }
}

void BoneRefObject::_connectBone(SkeletonObject& bone)
{
  emit boneRefChanged(&bone);
}

void BoneRefObject::_disconnectBone(SkeletonObject& bone) noexcept
{
  emit boneRefChanged(nullptr);
}

void BoneRefObject::setBoneInverseMatrix(const glm::mat4& newValue)
{
  if(_boneInverseMatrix == newValue) return;
  _boneInverseMatrix = newValue;
  emit boneInverseMatrixChanged(_boneInverseMatrix);
}
