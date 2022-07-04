#pragma once

#include <Objects/ModificatorObject.h>

class GravityModificator : public ModificatorObject
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(PEVisitorExtension,
                          visitGravityModificator,
                          visitConstGravityModificator,
                          ModificatorObject)

  Q_PROPERTY( float acceleration
              READ acceleration
              WRITE setAcceleration
              NOTIFY accelerationChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

public:
  GravityModificator( const QString& name,
                      bool canBeRenamed,
                      const mtt::UID& id = mtt::UID());
  GravityModificator(const GravityModificator&) = delete;
  GravityModificator& operator = (const GravityModificator&) = delete;
  virtual ~GravityModificator() noexcept = default;

  inline float acceleration() const noexcept;
  void setAcceleration(float newValue) noexcept;

  virtual void simulationStep(mtt::TimeRange time) override;

signals:
  void accelerationChanged(float newValue);

private:
  float _acceleration;
};

inline float GravityModificator::acceleration() const noexcept
{
  return _acceleration;
}
