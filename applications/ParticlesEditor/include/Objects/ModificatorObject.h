#pragma once

#include <mtt/editorLib/Objects/ScalableObject.h>

#include <mtt/application/Scene/ObjectLink.h>
#include <mtt/application/Application.h>

#include <Objects/ParticleField.h>
#include <Objects/PEVisitorExtension.h>

class ModificatorObject : public mtt::ScalableObject
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(PEVisitorExtension,
                          visitModificatorObject,
                          visitConstModificatorObject,
                          mtt::MovableObject)

  Q_PROPERTY( bool enabled
              READ enabled
              WRITE setEnabled
              RESET resetEnabled
              NOTIFY enabledChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( uint32_t typeMask
              READ typeMask
              WRITE setTypeMask
              RESET resetTypeMask
              NOTIFY typeMaskChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

public:
  using TimeType = mtt::Application::TimeType;

public:
  ModificatorObject(const QString& name,
                    bool canBeRenamed,
                    const mtt::UID& id = mtt::UID());
  ModificatorObject(const ModificatorObject&) = delete;
  ModificatorObject& operator = (const ModificatorObject&) = delete;
  virtual ~ModificatorObject() noexcept = default;

  inline bool enabled() const noexcept;
  void setEnabled(bool newValue) noexcept;
  inline void resetEnabled() noexcept;

  inline uint32_t typeMask() const noexcept;
  void setTypeMask(uint32_t newValue) noexcept;
  inline void resetTypeMask() noexcept;

  inline mtt::ObjectRef<ParticleField>& fieldRef() noexcept;
  inline const mtt::ObjectRef<ParticleField>& fieldRef() const noexcept;

  virtual void simulationStep(TimeType currentTime, TimeType delta);

signals:
  void enabledChanged(bool newValue);
  void typeMaskChanged(uint32_t newValue);
  void fieldChanged(ParticleField* newField);

private:
  void _connectToField(ParticleField& field);
  void _disconnectFromField(ParticleField& field) noexcept;

private:
  bool _enabled;
  uint32_t _typeMask;
  mtt::ObjectLink<ParticleField,
                  ModificatorObject,
                  &ModificatorObject::_connectToField,
                  &ModificatorObject::_disconnectFromField,
                  &ModificatorObject::fieldChanged> _fieldRef;
};

inline bool ModificatorObject::enabled() const noexcept
{
  return _enabled;
}

inline void ModificatorObject::resetEnabled() noexcept
{
  setEnabled(true);
}

inline uint32_t ModificatorObject::typeMask() const noexcept
{
  return _typeMask;
}

inline void ModificatorObject::resetTypeMask() noexcept
{
  setTypeMask(1);
}

inline mtt::ObjectRef<ParticleField>& ModificatorObject::fieldRef() noexcept
{
  return _fieldRef;
}

inline const mtt::ObjectRef<ParticleField>&
                                    ModificatorObject::fieldRef() const noexcept
{
  return _fieldRef;
}
