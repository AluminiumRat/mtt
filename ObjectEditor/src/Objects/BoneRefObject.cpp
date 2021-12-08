#include <mtt/Utilities/Abort.h>

#include <Objects/BoneRefObject.h>

BoneRefObject::BoneRefObject( const QString& name,
                              bool canBeRenamed,
                              const mtt::UID& id) :
  Object(name, canBeRenamed, id),
  _bone(nullptr),
  _boneInverseMatrix(1)
{
  _link.emplace(mtt::UID(), *this);
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
    _link.emplace(id, *this);
  }
  catch (...)
  {
    try
    {
      _link.emplace(mtt::UID(), *this);
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
  _bone = &bone;
  emit boneRefChanged(&bone);
}

void BoneRefObject::_disconnectBone(SkeletonObject& bone) noexcept
{
  _bone = nullptr;
  emit boneRefChanged(nullptr);
}

void BoneRefObject::setBoneInverseMatrix(const glm::mat4& newValue)
{
  if(_boneInverseMatrix == newValue) return;
  _boneInverseMatrix = newValue;
  emit boneInverseMatrixChanged(_boneInverseMatrix);
}
