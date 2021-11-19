#pragma once

#include <mtt/Core/Scene/Object.h>

#include <Objects/OEVisitorExtension.h>

class DisplayedObject : public mtt::Object
{
  Q_OBJECT

  Q_PROPERTY( bool visible
              READ visible
              WRITE setVisible
              RESET resetVisible
              NOTIFY visibleChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  DEFINE_EXTENSION_ACCEPT(OEVisitorExtension)

public:
  explicit DisplayedObject(const mtt::UID& id = mtt::UID());
  DisplayedObject(const DisplayedObject&) = delete;
  DisplayedObject& operator = (const DisplayedObject&) = delete;
  virtual ~DisplayedObject() noexcept = default;

  inline bool visible() const noexcept;
  void setVisible(bool newValue) noexcept;
  inline void resetVisible() noexcept;

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

inline void DisplayedObject::resetVisible() noexcept
{
  setVisible(true);
}

inline bool DisplayedObject::parentVisible() const noexcept
{
  return _parentVisible;
}

inline bool DisplayedObject::completeVisible() const noexcept
{
  return _completeVisible;
}
