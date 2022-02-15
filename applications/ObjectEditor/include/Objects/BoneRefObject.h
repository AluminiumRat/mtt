#pragma once

#include <glm/mat4x4.hpp>

#include <mtt/application/Scene/ObjectLink.h>
#include <mtt/application/Scene/Object.h>
#include <mtt/editorLib/Objects/SkeletonObject.h>

#include <Objects/OEVisitorExtension.h>

class BoneRefObject : public mtt::Object
{
  Q_OBJECT

  Q_PROPERTY( glm::mat4 boneInverseMatrix
              READ boneInverseMatrix
              WRITE setBoneInverseMatrix
              NOTIFY boneInverseMatrixChanged
              DESIGNABLE false
              SCRIPTABLE false
              STORED true
              USER false)

  DEFINE_EXTENSION_ACCEPT(OEVisitorExtension)

public:
  BoneRefObject(const QString& name,
                bool canBeRenamed,
                const mtt::UID& id = mtt::UID());
  BoneRefObject(const BoneRefObject&) = delete;
  BoneRefObject& operator = (const BoneRefObject&) = delete;
  virtual ~BoneRefObject() noexcept = default;

  inline const mtt::ObjectRef<mtt::SkeletonObject>& boneRef() const noexcept;
  inline mtt::SkeletonObject* bone() const noexcept;
  /// You can use nullptr to remove link
  void setBone(mtt::SkeletonObject* bone);
  /// You can use invalid UID to remove link
  void setBoneId(const mtt::UID& id);

  inline const glm::mat4& boneInverseMatrix() const noexcept;
  void setBoneInverseMatrix(const glm::mat4& newValue);

signals:
  void boneRefChanged(mtt::SkeletonObject* bone);
  void boneInverseMatrixChanged(const glm::mat4& newValue);

private:
  void _connectBone(mtt::SkeletonObject& bone);
  void _disconnectBone(mtt::SkeletonObject& bone) noexcept;

private:
  using BoneLink = mtt::ObjectLink< mtt::SkeletonObject,
                                    BoneRefObject,
                                    &BoneRefObject::_connectBone,
                                    &BoneRefObject::_disconnectBone>;
  BoneLink _link;

  glm::mat4 _boneInverseMatrix;
};

inline const mtt::ObjectRef<mtt::SkeletonObject>&
                                        BoneRefObject::boneRef() const noexcept
{
  return _link;
}

inline mtt::SkeletonObject* BoneRefObject::bone() const noexcept
{
  return _link.get();
}

inline const glm::mat4& BoneRefObject::boneInverseMatrix() const noexcept
{
  return _boneInverseMatrix;
}
