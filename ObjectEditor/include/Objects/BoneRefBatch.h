#pragma once

#include <memory>
#include <vector>

#include <mtt/Application/Scene/Object.h>

#include <Objects/BoneRefObject.h>

class BoneRefBatch : public mtt::Object
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(OEVisitorExtension)

public:
  BoneRefBatch( std::vector<std::unique_ptr<BoneRefObject>>&& refs,
                const QString& name,
                bool canBeRenamed,
                const mtt::UID& id = mtt::UID());
  BoneRefBatch(const BoneRefBatch&) = delete;
  BoneRefBatch& operator = (const BoneRefBatch&) = delete;
  virtual ~BoneRefBatch() noexcept = default;

  inline size_t boneRefsNumber() const noexcept;
  inline BoneRefObject& boneRef(size_t index) noexcept;
  inline const BoneRefObject& boneRef(size_t index) const noexcept;

private:
  std::vector<BoneRefObject*> _refs;
};

inline size_t BoneRefBatch::boneRefsNumber() const noexcept
{
  return _refs.size();
}

inline BoneRefObject& BoneRefBatch::boneRef(size_t index) noexcept
{
  return *_refs[index];
}

inline const BoneRefObject& BoneRefBatch::boneRef(size_t index) const noexcept
{
  return *_refs[index];
}
