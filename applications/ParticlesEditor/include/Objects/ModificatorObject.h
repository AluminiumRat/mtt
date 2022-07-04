#pragma once

#include <mtt/application/Scene/ObjectLink.h>
#include <mtt/application/TimeT.h>

#include <Objects/HierarhicalObject.h>
#include <Objects/ParticleField.h>
#include <Objects/PEVisitorExtension.h>

class ModificatorObject : public HierarhicalObject
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(PEVisitorExtension,
                          visitModificatorObject,
                          visitConstModificatorObject,
                          HierarhicalObject)

  Q_PROPERTY( bool enabled
              READ enabled
              WRITE setEnabled
              NOTIFY enabledChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( uint32_t typeMask
              READ typeMask
              WRITE setTypeMask
              NOTIFY typeMaskChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

public:
  ModificatorObject(const QString& name,
                    bool canBeRenamed,
                    ParticleField::ModificatorApplyTime applyTime,
                    const mtt::UID& id = mtt::UID());
  ModificatorObject(const ModificatorObject&) = delete;
  ModificatorObject& operator = (const ModificatorObject&) = delete;
  virtual ~ModificatorObject() noexcept = default;

  inline bool enabled() const noexcept;
  void setEnabled(bool newValue) noexcept;

  inline ParticleField::ModificatorApplyTime applyTime() const noexcept;

  inline uint32_t typeMask() const noexcept;
  void setTypeMask(uint32_t newValue) noexcept;

  inline mtt::ObjectRef<ParticleField>& fieldRef() noexcept;
  inline const mtt::ObjectRef<ParticleField>& fieldRef() const noexcept;

  virtual void simulationStep(mtt::TimeRange time);

signals:
  void enabledChanged(bool newValue);
  void typeMaskChanged(uint32_t newValue);
  void fieldChanged(ParticleField* newField);

protected:
  virtual void connectToField(ParticleField& field);
  virtual void disconnectFromField(ParticleField& field) noexcept;

private:
  ParticleField::ModificatorApplyTime _applyTime;
  bool _enabled;
  uint32_t _typeMask;
  mtt::ObjectLink<ParticleField,
                  ModificatorObject,
                  &ModificatorObject::connectToField,
                  &ModificatorObject::disconnectFromField,
                  &ModificatorObject::fieldChanged> _fieldRef;
};

inline bool ModificatorObject::enabled() const noexcept
{
  return _enabled;
}

inline ParticleField::ModificatorApplyTime
                                  ModificatorObject::applyTime() const noexcept
{
  return _applyTime;
}

inline uint32_t ModificatorObject::typeMask() const noexcept
{
  return _typeMask;
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
