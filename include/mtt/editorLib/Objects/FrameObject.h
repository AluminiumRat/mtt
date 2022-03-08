#pragma once

#include <mtt/editorLib/Objects/ScalableObject.h>

namespace mtt
{
  class FrameObject : public ScalableObject
  {
    Q_OBJECT
    DEFINE_EXTENSION_ACCEPT(CEVisitorExtension,
                            visitFrameObject,
                            visitConstFrameObject,
                            ScalableObject)

  public:
    FrameObject( const QString& name,
                    bool canBeRenamed,
                    const UID& id = UID());
    FrameObject(const FrameObject&) = delete;
    FrameObject& operator = (const FrameObject&) = delete;
    virtual ~FrameObject() noexcept = default;

    inline size_t childsNumber() const noexcept;
    inline Object3D& child(size_t childIndex) noexcept;
    inline const Object3D& child(size_t childIndex) const noexcept;

    virtual void addChild(std::unique_ptr<Object3D> object);
    virtual std::unique_ptr<Object3D> removeChild(
                                                Object3D& object,
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
    void childAdded(Object3D& object);
    void childRemoved(Object3D& object);
  };

  inline size_t FrameObject::childsNumber() const noexcept
  {
    return subobjectNumber();
  }

  inline Object3D& FrameObject::child(size_t childIndex) noexcept
  {
    return static_cast<Object3D&>(subobject(childIndex));
  }

  inline const Object3D& FrameObject::child(
                                              size_t childIndex) const noexcept
  {
    return static_cast<const Object3D&>(subobject(childIndex));
  }
}