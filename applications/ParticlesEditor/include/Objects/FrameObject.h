#pragma once

#include <Objects/HierarhicalObject.h>

class FrameObject : public HierarhicalObject
{
  Q_OBJECT
  DEFINE_EXTENSION_ACCEPT(PEVisitorExtension,
                          visitFrameObject,
                          visitConstFrameObject,
                          HierarhicalObject)

public:
  FrameObject(const QString& name,
              bool canBeRenamed,
              const mtt::UID& id = mtt::UID());
  FrameObject(const FrameObject&) = delete;
  FrameObject& operator = (const FrameObject&) = delete;
  virtual ~FrameObject() noexcept = default;

  inline size_t childsNumber() const noexcept;
  inline HierarhicalObject& child(size_t childIndex) noexcept;
  inline const HierarhicalObject& child(size_t childIndex) const noexcept;

  virtual void addChild(std::unique_ptr<HierarhicalObject> object);
  virtual std::unique_ptr<HierarhicalObject> removeChild(
                                              HierarhicalObject& object,
                                              bool removeFromScene) noexcept;

  virtual bool subobjectCanBeAdded(
                          const mtt::Object& subobject) const noexcept override;
  virtual std::unique_ptr<mtt::Object> tryAddSubobject(
                                  std::unique_ptr<mtt::Object> object) override;
  virtual bool subobjectCanBeRemoved(
                          const mtt::Object& subobject) const noexcept override;
  virtual std::unique_ptr<mtt::Object> tryRemoveSubobject(
                                      mtt::Object& subobject,
                                      bool removeFromScene) noexcept override;

signals:
  void childAdded(HierarhicalObject& object);
  void childRemoved(HierarhicalObject& object);
};

inline size_t FrameObject::childsNumber() const noexcept
{
  return subobjectNumber();
}

inline HierarhicalObject& FrameObject::child(size_t childIndex) noexcept
{
  return static_cast<HierarhicalObject&>(subobject(childIndex));
}

inline const HierarhicalObject& FrameObject::child(
                                            size_t childIndex) const noexcept
{
  return static_cast<const HierarhicalObject&>(subobject(childIndex));
}
