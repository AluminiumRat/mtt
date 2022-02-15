#pragma once

#include <mtt/editorLib/Objects/ScalableObject.h>

namespace mtt
{
  class SkeletonObject : public ScalableObject
  {
    Q_OBJECT
    DEFINE_EXTENSION_ACCEPT(CEVisitorExtension)

  public:
    SkeletonObject( const QString& name,
                    bool canBeRenamed,
                    const UID& id = UID());
    SkeletonObject(const SkeletonObject&) = delete;
    SkeletonObject& operator = (const SkeletonObject&) = delete;
    virtual ~SkeletonObject() noexcept = default;

    inline size_t childsNumber() const noexcept;
    inline SkeletonObject& child(size_t childIndex) noexcept;
    inline const SkeletonObject& child(size_t childIndex) const noexcept;

    virtual void addChild(std::unique_ptr<SkeletonObject> object);
    virtual std::unique_ptr<SkeletonObject> removeChild(
                                                SkeletonObject& object,
                                                bool removeFromScene) noexcept;

    virtual bool subobjectCanBeAdded(
                              const Object& subobject) const noexcept override;
    virtual std::unique_ptr<Object> tryAddSubobject(
                                      std::unique_ptr<Object> object) override;
    virtual bool subobjectCanBeRemoved(
                              const Object& subobject) const noexcept override;
    virtual std::unique_ptr<Object> tryRemoveSubobject(
                                        Object& subobject,
                                        bool removeFromScene) noexcept override;

  signals:
    void childAdded(SkeletonObject& object);
    void childRemoved(SkeletonObject& object);
  };

  inline size_t SkeletonObject::childsNumber() const noexcept
  {
    return subobjectNumber();
  }

  inline SkeletonObject& SkeletonObject::child(size_t childIndex) noexcept
  {
    return static_cast<SkeletonObject&>(subobject(childIndex));
  }

  inline const SkeletonObject& SkeletonObject::child(
                                              size_t childIndex) const noexcept
  {
    return static_cast<const SkeletonObject&>(subobject(childIndex));
  }
}