#pragma once

#include <optional>

#include <glm/mat4x4.hpp>

#include <mtt/Application/Scene/ObjectLink.h>
#include <mtt/Application/Scene/Object.h>

#include <Objects/OEVisitorExtension.h>
#include <Objects/SkeletonObject.h>

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

  inline const mtt::ObjectRef<SkeletonObject>& boneRef() const noexcept;
  inline SkeletonObject* bone() const noexcept;
  /// You can use nullptr to remove link
  void setBone(SkeletonObject* bone);
  /// You can use invalid UID to remove link
  void setBoneId(const mtt::UID& id);

  inline const glm::mat4& boneInverseMatrix() const noexcept;
  void setBoneInverseMatrix(const glm::mat4& newValue);

signals:
  void boneRefChanged(SkeletonObject* bone);
  void boneInverseMatrixChanged(const glm::mat4& newValue);

private:
  void _connectBone(SkeletonObject& bone);
  void _disconnectBone(SkeletonObject& bone) noexcept;

private:
  using BoneLink = mtt::ObjectLink< SkeletonObject,
                                    BoneRefObject,
                                    &BoneRefObject::_connectBone,
                                    &BoneRefObject::_disconnectBone>;
  std::optional<BoneLink> _link;
  SkeletonObject* _bone;

  glm::mat4 _boneInverseMatrix;
};

inline const mtt::ObjectRef<SkeletonObject>&
                                        BoneRefObject::boneRef() const noexcept
{
  return *_link;
}

inline SkeletonObject* BoneRefObject::bone() const noexcept
{
  return _bone;
}

inline const glm::mat4& BoneRefObject::boneInverseMatrix() const noexcept
{
  return _boneInverseMatrix;
}
