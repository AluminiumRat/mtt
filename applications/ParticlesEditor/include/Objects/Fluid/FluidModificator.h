#pragma once

#include <vector>

#include <Objects/ModificatorObject.h>

class FluidModificator : public ModificatorObject
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(PEVisitorExtension,
                          visitFluidModificator,
                          visitConstFluidModificator,
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

public:
  FluidModificator( const QString& name,
                    bool canBeRenamed,
                    const mtt::UID& id = mtt::UID());
  FluidModificator(const FluidModificator&) = delete;
  FluidModificator& operator = (const FluidModificator&) = delete;
  virtual ~FluidModificator() noexcept = default;

  inline float size() const noexcept;
  void setSize(float newValue) noexcept;
  inline void resetSize() noexcept;

signals:
  void sizeChanged(float newValue);

protected:
  struct CellRecord
  {
    size_t x, y, z;
    float weight;
  };
  using Cells = std::vector<CellRecord>;
  Cells collectCells() const;

private:
  float _size;
};

inline float FluidModificator::size() const noexcept
{
  return _size;
}

inline void FluidModificator::resetSize() noexcept
{
  setSize(1.f);
}
