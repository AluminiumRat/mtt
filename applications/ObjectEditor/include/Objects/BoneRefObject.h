#pragma once

#include <glm/mat4x4.hpp>

#include <mtt/application/Scene/ObjectLink.h>
#include <mtt/application/Scene/Object.h>
#include <mtt/editorLib/Objects/SkeletonObject.h>

#include <Objects/OEVisitorExtension.h>

class BoneRefObject : public mtt::Object
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(OEVisitorExtension,
                          visitBoneRefObject,
                          visitConstBoneRefObject,
                          mtt::Object)

  Q_PROPERTY( glm::mat4 boneInverseMatrix
              READ boneInverseMatrix
              WRITE setBoneInverseMatrix
              NOTIFY boneInverseMatrixChanged
              DESIGNABLE false
              SCRIPTABLE false
              STORED true
              USER false)

public:
  BoneRefObject(const QString& name,
                bool canBeRenamed,
                const mtt::UID& id = mtt::UID());
  BoneRefObject(const BoneRefObject&) = delete;
  BoneRefObject& operator = (const BoneRefObject&) = delete;
  virtual ~BoneRefObject() noexcept = default;

  inline mtt::ObjectRef<mtt::SkeletonObject>& boneRef() noexcept;
  inline const mtt::ObjectRef<mtt::SkeletonObject>& boneRef() const noexcept;

  inline const glm::mat4& boneInverseMatrix() const noexcept;
  void setBoneInverseMatrix(const glm::mat4& newValue);

signals:
  void boneRefChanged(mtt::SkeletonObject* bone);
  void boneInverseMatrixChanged(const glm::mat4& newValue);

private:
  using BoneLink = mtt::ObjectLink< mtt::SkeletonObject,
                                    BoneRefObject,
                                    nullptr,
                                    nullptr,
                                    &BoneRefObject::boneRefChanged>;
  BoneLink _link;

  glm::mat4 _boneInverseMatrix;
};

inline mtt::ObjectRef<mtt::SkeletonObject>& BoneRefObject::boneRef() noexcept
{
  return _link;
}

inline const mtt::ObjectRef<mtt::SkeletonObject>&
                                        BoneRefObject::boneRef() const noexcept
{
  return _link;
}

inline const glm::mat4& BoneRefObject::boneInverseMatrix() const noexcept
{
  return _boneInverseMatrix;
}
