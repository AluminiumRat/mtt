#include <Objects/LODObject.h>

LODObject::LODObject( const QString& name,
                      bool canBeRenamed,
                      const mtt::UID& id) :
  SpecialGroup(name, canBeRenamed, id),
  _minMppx(0),
  _maxMppx(FLT_MAX)
{
}

void LODObject::setMinMppx(float newValue) noexcept
{
  if(_minMppx == newValue) return;
  _minMppx = newValue;
  emit minMppxChanged(newValue);
  emit rangeChanged(_minMppx, _maxMppx);
}

void LODObject::setMaxMppx(float newValue) noexcept
{
  if (_maxMppx == newValue) return;
  _maxMppx = newValue;
  emit maxMppxChanged(newValue);
  emit rangeChanged(_minMppx, _maxMppx);
}

void LODObject::setRange(float newMinMppx, float newMaxMppx) noexcept
{
  if(_minMppx == newMinMppx && _maxMppx == newMaxMppx) return;
  _minMppx = newMinMppx;
  _maxMppx = newMaxMppx;
  emit minMppxChanged(_minMppx);
  emit maxMppxChanged(_maxMppx);
  emit rangeChanged(_minMppx, _maxMppx);
}
