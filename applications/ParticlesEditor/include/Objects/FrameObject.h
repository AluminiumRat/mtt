#pragma once

#include <mtt/editorLib/Objects/ScalableObject.h>

#include <Objects/ModificatorObject.h>
#include <Objects/PEVisitorExtension.h>

class FrameObject : public ModificatorObject
{
  Q_OBJECT
  DEFINE_EXTENSION_ACCEPT(PEVisitorExtension,
                          visitFrameObject,
                          visitConstFrameObject,
                          ModificatorObject)

public:
  FrameObject(const QString& name,
              bool canBeRenamed,
              const mtt::UID& id = mtt::UID());
  FrameObject(const FrameObject&) = delete;
  FrameObject& operator = (const FrameObject&) = delete;
  virtual ~FrameObject() noexcept = default;

  inline size_t childsNumber() const noexcept;
  inline ModificatorObject& child(size_t childIndex) noexcept;
  inline const ModificatorObject& child(size_t childIndex) const noexcept;

  virtual void addChild(std::unique_ptr<ModificatorObject> object);
  virtual std::unique_ptr<ModificatorObject> removeChild(
                                              ModificatorObject& object,
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
  void childAdded(ModificatorObject& object);
  void childRemoved(ModificatorObject& object);
};

inline size_t FrameObject::childsNumber() const noexcept
{
  return subobjectNumber();
}

inline ModificatorObject& FrameObject::child(size_t childIndex) noexcept
{
  return static_cast<ModificatorObject&>(subobject(childIndex));
}

inline const ModificatorObject& FrameObject::child(
                                            size_t childIndex) const noexcept
{
  return static_cast<const ModificatorObject&>(subobject(childIndex));
}
