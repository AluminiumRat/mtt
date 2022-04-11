#pragma once

#include <mtt/application/Scene/Object.h>
#include <mtt/editorLib/Objects/CEVisitorExtension.h>
#include <mtt/editorLib/Objects/BackgroundObject.h>
#include <mtt/editorLib/Objects/EnvironmentGroup.h>

namespace mtt
{
  class EnvironmentRootObject : public Object
  {
    Q_OBJECT
    DEFINE_EXTENSION_ACCEPT(CEVisitorExtension,
                            visitEnvironmentRootObject,
                            visitConstEnvironmentRootObject,
                            Object)

  public:
    EnvironmentRootObject(const QString& name,
                          bool canBeRenamed,
                          const mtt::UID& id = mtt::UID());
    EnvironmentRootObject(const EnvironmentRootObject&) = delete;
    EnvironmentRootObject& operator = (const EnvironmentRootObject&) = delete;
    virtual ~EnvironmentRootObject() noexcept = default;

    inline mtt::BackgroundObject& background() noexcept;
    inline const mtt::BackgroundObject& background() const noexcept;
    /// newBackground must not be nullptr
    void changeBackground(std::unique_ptr<mtt::BackgroundObject> newBackground);

    inline mtt::EnvironmentGroup& objects() noexcept;
    inline const mtt::EnvironmentGroup& objects() const noexcept;

  signals:
    void backgroundChanged(mtt::BackgroundObject& newBackground);

  private:
    mtt::BackgroundObject* _background;
    mtt::EnvironmentGroup* _objects;
  };

  inline mtt::BackgroundObject& EnvironmentRootObject::background() noexcept
  {
    return *_background;
  }

  inline const mtt::BackgroundObject&
                              EnvironmentRootObject::background() const noexcept
  {
    return *_background;
  }

  inline mtt::EnvironmentGroup& EnvironmentRootObject::objects() noexcept
  {
    return *_objects;
  }

  inline const mtt::EnvironmentGroup&
                                EnvironmentRootObject::objects() const noexcept
  {
    return *_objects;
  }
}