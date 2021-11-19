#pragma once

#include <vector>

#include <QtCore/QObject>

#include <glm/mat4x4.hpp>

namespace mtt
{
  class MeshExtraData;
}

class BoneRefBatch;
class MeshObject;
class SkeletonObject;

class BonesObserver : public QObject
{
  Q_OBJECT

public:
  BonesObserver(MeshObject& object,
                BoneRefBatch& boneBatch,
                mtt::MeshExtraData& extraData);
  BonesObserver(const BonesObserver&) = delete;
  BonesObserver& operator = (const BonesObserver&) = delete;
  virtual ~BonesObserver() noexcept;

private:
  void _onMeshMoved() noexcept;
  void _setBoneObject(SkeletonObject* boneObject,
                      size_t boneIndex) noexcept;
  void _setBoneMatrix(SkeletonObject* boneObject,
                      size_t boneIndex) noexcept;
  void _setInverseMatrix( const glm::mat4& newValue,
                          size_t boneIndex,
                          bool updateExtraData) noexcept;

private:
  MeshObject& _object;
  BoneRefBatch& _boneBatch;
  mtt::MeshExtraData& _extraData;

  struct BoneRecord
  {
    SkeletonObject* bone = nullptr;
    QMetaObject::Connection refConnection;
    QMetaObject::Connection inverseMatrixConnection;
    QMetaObject::Connection transformConnection;
  };
  std::vector<BoneRecord> _bones;
  
  std::vector<glm::mat4> _boneMatrices;
  std::vector<glm::mat4> _inverseBoneMatrices;
};