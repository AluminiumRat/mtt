#pragma once

#include <vector>

#include <Objects/ModificatorObject.h>

class HeaterObject : public ModificatorObject
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(PEVisitorExtension,
                          visitHeaterObject,
                          visitConstHeaterObject,
                          ModificatorObject)

  Q_PROPERTY( float size
              READ size
              WRITE setSize
              RESET resetSize
              NOTIFY sizeChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( float power
              READ power
              WRITE setPower
              RESET resetPower
              NOTIFY powerChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

public:
  HeaterObject( const QString& name,
                bool canBeRenamed,
                const mtt::UID& id = mtt::UID());
  HeaterObject(const HeaterObject&) = delete;
  HeaterObject& operator = (const HeaterObject&) = delete;
  virtual ~HeaterObject() noexcept = default;

  inline float size() const noexcept;
  void setSize(float newValue) noexcept;
  inline void resetSize() noexcept;

  inline float power() const noexcept;
  void setPower(float newValue) noexcept;
  inline void resetPower() noexcept;

  virtual void simulationStep(mtt::TimeT currentTime,
                              mtt::TimeT deltaT) override;

signals:
  void sizeChanged(float newValue);
  void powerChanged(float newValue);

private:
  struct CellRecord
  {
    size_t x, y, z;
    float weight;
  };
  using Cells = std::vector<CellRecord>;
  void collectCells(Cells& targetCells, float& totalWeight) const;

private:
  float _size;
  float _power;
};

inline float HeaterObject::size() const noexcept
{
  return _size;
}

inline void HeaterObject::resetSize() noexcept
{
  setSize(1.f);
}

inline float HeaterObject::power() const noexcept
{
  return _power;
}

inline void HeaterObject::resetPower() noexcept
{
  setPower(1000.f);
}
