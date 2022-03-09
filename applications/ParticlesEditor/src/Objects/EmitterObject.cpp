#include <Objects/EmitterObject.h>

const std::map<EmitterObject::Shape, QString> EmitterObject::shapeNames =
  { {EmitterObject::CIRCLE_SHAPE, QT_TR_NOOP("Circle")},
    {EmitterObject::SPHERE_SHAPE, QT_TR_NOOP("Sphere")}};

const std::map<EmitterObject::Distribution, QString>
                                              EmitterObject::distributionNames =
  { {EmitterObject::UNIFORM_DISTRIBUTION, QT_TR_NOOP("Uniform")},
    {EmitterObject::SMOOTH_DISTRIBUTION, QT_TR_NOOP("Smooth")}};

EmitterObject::EmitterObject( const QString& name,
                              bool canBeRenamed,
                              const mtt::UID& id) :
  ModificatorObject(name, canBeRenamed, id),
  _intensity(0.f),
  _size(1.f),
  _shape(SPHERE_SHAPE),
  _distribution(UNIFORM_DISTRIBUTION)
{
}

void EmitterObject::setIntensity(float newValue) noexcept
{
  if(_intensity == newValue) return;
  _intensity = newValue;
  emit intensityChanged(newValue);
}

void EmitterObject::setSize(float newValue) noexcept
{
  if(_size == newValue) return;
  _size = newValue;
  emit sizeChanged(newValue);
}

void EmitterObject::setShape(Shape newValue) noexcept
{
  if(_shape == newValue) return;
  _shape = newValue;
  emit shapeChanged(newValue);
}

void EmitterObject::setDistribution(Distribution newValue) noexcept
{
  if(_distribution == newValue) return;
  _distribution = newValue;
  emit distributionChanged(newValue);
}
