#pragma once

#include <mtt/editorLib/Objects/ScalableObject.h>

#include <mtt/application/Scene/ObjectLink.h>

#include <Objects/ParticleField.h>
#include <Objects/PEVisitorExtension.h>

class ModificatorObject : public mtt::ScalableObject
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(PEVisitorExtension,
                          visitModificatorObject,
                          visitConstModificatorObject,
                          mtt::MovableObject)

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
  ModificatorObject(const QString& name,
                    bool canBeRenamed,
                    const mtt::UID& id = mtt::UID());
  ModificatorObject(const ModificatorObject&) = delete;
  ModificatorObject& operator = (const ModificatorObject&) = delete;
  virtual ~ModificatorObject() noexcept = default;

  inline uint32_t typeMask() const noexcept;
  void setTypeMask(uint32_t newValue) noexcept;
  inline void resetTypeMask() noexcept;

  inline mtt::ObjectRef<ParticleField>& fieldRef() noexcept;
  inline const mtt::ObjectRef<ParticleField>& fieldRef() const noexcept;

signals:
  void typeMaskChanged(uint32_t newValue);

private:
  uint32_t _typeMask;
  mtt::ObjectLink<ParticleField, ModificatorObject, nullptr, nullptr> _fieldRef;
};

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
