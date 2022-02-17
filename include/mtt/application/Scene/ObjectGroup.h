#pragma once

#include <mtt/application/Scene/Object.h>

namespace mtt
{
  class ObjectGroup : public Object
  {
    Q_OBJECT

    DEFINE_VISITOR_ACCEPT(visitObjectGroup, visitConstObjectGroup)

  public:
    ObjectGroup(const QString& name, bool canBeRenamed, const UID& id = UID());
    ObjectGroup(const ObjectGroup&) = delete;
    ObjectGroup& operator = (const ObjectGroup&) = delete;
    virtual ~ObjectGroup() noexcept = default;

    inline size_t childsNumber() const noexcept;
    inline Object& child(size_t childIndex) noexcept;
    inline const Object& child(size_t childIndex) const noexcept;

    virtual void addChild(std::unique_ptr<Object> object);
    virtual std::unique_ptr<Object> removeChild(Object& object,
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
    void childAdded(Object& object);
    void childRemoved(Object& object);
  };

  inline size_t ObjectGroup::childsNumber() const noexcept
  {
    return subobjectNumber();
  }

  inline Object& ObjectGroup::child(size_t childIndex) noexcept
  {
    return subobject(childIndex);
  }

  inline const Object& ObjectGroup::child(size_t childIndex) const noexcept
  {
    return subobject(childIndex);
  }
}