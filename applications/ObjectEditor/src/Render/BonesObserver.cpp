#include <stdexcept>

#include <mtt/render/Mesh/MeshExtraData.h>

#include <mtt/utilities/Log.h>

#include <mtt/render/Drawable/BoneMatricesSetter.h>

#include <Objects/BoneRefBatch.h>
#include <Objects/MeshObject.h>
#include <Render/BonesObserver.h>

BonesObserver::BonesObserver( MeshObject& object,
                              BoneRefBatch& boneBatch,
                              mtt::MeshExtraData& extraData,
                              mtt::BoneMatricesSetter& matricesSetter) :
  _object(object),
  _boneBatch(boneBatch),
  _extraData(extraData),
  _matricesSetter(matricesSetter)
{
  size_t bonesNumber(_boneBatch.boneRefsNumber());
  if(bonesNumber != 0)
  {
    _matricesSetter.matrices.resize(bonesNumber);
    _inverseBoneMatrices.resize(bonesNumber);
    _bones.resize(bonesNumber);

    for(size_t boneIndex = 0; boneIndex < bonesNumber; boneIndex++)
    {
      BoneRefObject& boneRef = _boneBatch.boneRef(boneIndex);
      _bones[boneIndex].refConnection=
                              connect(&boneRef,
                                      &BoneRefObject::boneRefChanged,
                                      [&, boneIndex](mtt::SkeletonObject* bone)
                                      {
                                        _setBoneObject(bone, boneIndex);
                                      });
      _setBoneObject(boneRef.boneRef().get(), boneIndex);

      _bones[boneIndex].inverseMatrixConnection =
                        connect(&boneRef,
                                &BoneRefObject::boneInverseMatrixChanged,
                                [&, boneIndex](const glm::mat4& newValue)
                                {
                                  _setInverseMatrix(newValue, boneIndex, true);
                                });
      _setInverseMatrix(boneRef.boneInverseMatrix(), boneIndex, false);
    }

    _extraData.setBoneInverseMatricesData(_inverseBoneMatrices);

    connect(&_object,
            &MeshObject::transformChanged,
            this,
            &BonesObserver::_onMeshMoved,
            Qt::DirectConnection);
  }
}

BonesObserver::~BonesObserver()
{
  if (!_bones.empty())
  {
    for(BoneRecord& record : _bones)
    {
      if(record.refConnection) disconnect(record.refConnection);
      if(record.inverseMatrixConnection)
      {
        disconnect(record.inverseMatrixConnection);
      }
      if(record.transformConnection) disconnect(record.transformConnection);
    }

    _extraData.removeBoneInverseMatricesData();
  }
}

void BonesObserver::_onMeshMoved() noexcept
{
  for(size_t boneIndex = 0; boneIndex < _bones.size(); boneIndex++)
  {
    mtt::SkeletonObject* bone = _bones[boneIndex].bone;
    if(bone != nullptr) _setBoneMatrix(bone, boneIndex);
  }
}

void BonesObserver::_setBoneObject( mtt::SkeletonObject* boneObject,
                                    size_t boneIndex) noexcept
{
  mtt::SkeletonObject* oldBoneObject = _bones[boneIndex].bone;
  if(oldBoneObject != nullptr)
  {
    disconnect(_bones[boneIndex].transformConnection);
  }
  _bones[boneIndex].bone = boneObject;
  if(boneObject != nullptr)
  {
    _bones[boneIndex].transformConnection =
                                connect(boneObject,
                                        &mtt::SkeletonObject::transformChanged,
                                        [&, boneObject, boneIndex]()
                                        {
                                          _setBoneMatrix( boneObject,
                                                          boneIndex);
                                        });
    _setBoneMatrix(boneObject, boneIndex);
  }
}

void BonesObserver::_setBoneMatrix( mtt::SkeletonObject* boneObject,
                                    size_t boneIndex) noexcept
{
  const glm::mat4& meshTransform = _object.localToWorldTransform();
  const glm::mat4& boneTransform = boneObject->localToWorldTransform();
  _matricesSetter.matrices[boneIndex] =
                                    glm::inverse(meshTransform) * boneTransform;
}

void BonesObserver::_setInverseMatrix(const glm::mat4& newValue,
                                      size_t boneIndex,
                                      bool updateExtraData) noexcept
{
  _inverseBoneMatrices[boneIndex] = newValue;
  if(updateExtraData)
  {
    try
    {
      _extraData.setBoneInverseMatricesData(_inverseBoneMatrices);
    }
    catch (std::exception& error)
    {
      mtt::Log() << error.what();
      mtt::Log() << "BonesObserver::_setInverseMatrix: unable to update bone matrices";
    }
    catch (...)
    {
      mtt::Log() << "BonesObserver::_setInverseMatrix: unable to update bone matrices";
    }
  }
}
