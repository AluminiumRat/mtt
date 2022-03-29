#include <Objects/Fluid/FluidObject.h>
#include <Objects/BlockerObject.h>
#include <Objects/ParticleField.h>

const std::map<BlockerObject::Shape, QString> BlockerObject::shapeNames =
  { {BlockerObject::SPHERE_SHAPE, QT_TR_NOOP("Sphere")},
    {BlockerObject::BOX_SHAPE, QT_TR_NOOP("Box")},
    {BlockerObject::CYLINDER_SHAPE, QT_TR_NOOP("Cylinder")}};

BlockerObject::BlockerObject( const QString& name,
                              bool canBeRenamed,
                              const mtt::UID& id) :
  ModificatorObject(name, canBeRenamed, id),
  _shape(SPHERE_SHAPE),
  _size(1.f),
  _halfsize(.5f)
{
}

BlockerObject::~BlockerObject() noexcept
{
  if(fieldRef() != nullptr) _disconnectFromFluid(fieldRef()->fluid());
}

void BlockerObject::setShape(Shape newValue) noexcept
{
  if(_shape == newValue) return;
  _shape = newValue;
  emit shapeChanged(newValue);
}

void BlockerObject::setSize(float newValue) noexcept
{
  if(_size == newValue) return;
  _size = newValue;
  _halfsize = _size / 2.f;
  emit sizeChanged(newValue);
}

bool BlockerObject::isPointInside(const glm::vec3& point) noexcept
{
  switch (_shape)
  {
    case SPHERE_SHAPE:
      return glm::length(point) <= _halfsize;

    case BOX_SHAPE:
      if( point.x < -_halfsize ||
          point.x > _halfsize ||
          point.y < -_halfsize ||
          point.y > _halfsize || 
          point.z < -_halfsize ||
          point.z > _halfsize) return false;
      return true;

    case CYLINDER_SHAPE:
      if(glm::length(glm::vec2(point)) > _halfsize) return false;
      if(point.z < -_halfsize || point.z > _halfsize) return false;
      return true;
  }

  return false;
}

void BlockerObject::connectToField(ParticleField& field)
{
  ModificatorObject::connectToField(field);
  try
  {
    _connectToFluid(field.fluid());
  }
  catch (...)
  {
    ModificatorObject::disconnectFromField(field);
    throw;
  }
}

void BlockerObject::_connectToFluid(FluidObject& fluid)
{
  fluid.registerBlocker(*this);
}

void BlockerObject::disconnectFromField(ParticleField& field) noexcept
{
  ModificatorObject::disconnectFromField(field);
  _disconnectFromFluid(field.fluid());
}

void BlockerObject::_disconnectFromFluid(FluidObject& fluid) noexcept
{
  fluid.unregisterBlocker(*this);
}
