#pragma once

#include <mtt/application/Scene/Object.h>
#include <mtt/editorLib/Objects/CEVisitorExtension.h>

namespace mtt
{
  class DisplayedObject : public Object
  {
    Q_OBJECT
    DEFINE_EXTENSION_ACCEPT(CEVisitorExtension,
                            visitDisplayedObject,
                            visitConstDisplayedObject,
                            Object)

    Q_PROPERTY( bool visible
                READ visible
                WRITE setVisible
                NOTIFY visibleChanged
                DESIGNABLE true
                SCRIPTABLE true
                STORED true
                USER false)
  public:
    DisplayedObject(const QString& name,
                    bool canBeRenamed,
                    const UID& id = UID());
    DisplayedObject(const DisplayedObject&) = delete;
    DisplayedObject& operator = (const DisplayedObject&) = delete;
    virtual ~DisplayedObject() noexcept = default;

    inline bool visible() const noexcept;
    void setVisible(bool newValue) noexcept;

    inline bool parentVisible() const noexcept;
    void setParentVisible(bool newValue) noexcept;

    inline bool completeVisible() const noexcept;

  signals:
    void visibleChanged(bool newValue);
    void completeVisibleChanged(bool newValue);

  protected:
    virtual void onSubobjectAdded(Object& object) noexcept override;
    virtual void onSubobjectRemoved(Object& object) noexcept override;

  private:
    void _updateCompleteVisible() noexcept;

  private:
    bool _visible;
    bool _parentVisible;
    bool _completeVisible;
  };

  inline bool DisplayedObject::visible() const noexcept
  {
    return _visible;
  }

  inline bool DisplayedObject::parentVisible() const noexcept
  {
    return _parentVisible;
  }

  inline bool DisplayedObject::completeVisible() const noexcept
  {
    return _completeVisible;
  }
}