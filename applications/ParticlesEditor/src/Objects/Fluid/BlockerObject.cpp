#include <Objects/Fluid/BlockerObject.h>
#include <Objects/Fluid/FluidObject.h>
#include <Objects/ParticleField.h>

const std::map<BlockerObject::Shape, QString> BlockerObject::shapeNames =
  { {BlockerObject::SPHERE_SHAPE, QT_TR_NOOP("Sphere")},
    {BlockerObject::BOX_SHAPE, QT_TR_NOOP("Box")},
    {BlockerObject::CYLINDER_SHAPE, QT_TR_NOOP("Cylinder")}};

BlockerObject::BlockerObject( const QString& name,
                              bool canBeRenamed,
                              const mtt::UID& id) :
  ModificatorObject(name, canBeRenamed, ParticleField::POSTUPDATE_TIME, id),
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

void BlockerObject::simulationStep(mtt::TimeRange time)
{
  ParticleField* field = fieldRef().get();
  if (field == nullptr) return;

  glm::mat4x4 toField = glm::inverse(field->localToWorldTransform()) *
                                                        localToWorldTransform();
  glm::mat4x4 fromField = glm::inverse(toField);

  switch(_shape)
  {
  case SPHERE_SHAPE:
    {
      auto sphereDelegate =
      [&](ParticleField::ParticleData& particle)
      {
        glm::vec3 localPosition = fromField * glm::vec4(particle.position, 1.f);
        float distance = glm::length(localPosition);
        if (distance == 0.f) return;
        if (distance < _halfsize) localPosition *= _halfsize / distance;
        particle.position = toField * glm::vec4(localPosition, 1.f);
      };
      field->updateParticles(sphereDelegate, typeMask());
      break;
    }

  case BOX_SHAPE:
    {
      auto boxDelegate =
      [&](ParticleField::ParticleData& particle)
      {
        glm::vec3 localPosition = fromField * glm::vec4(particle.position, 1.f);
        if (isPointInside(localPosition))
        {
          float absX = abs(localPosition.x);
          float absY = abs(localPosition.y);
          float absZ = abs(localPosition.z);
          if (absX > absY && absX > absZ)
          {
            localPosition.x *= _halfsize / absX;
          }
          else if (absY > absZ)
          {
            localPosition.y *= _halfsize / absY;
          }
          else
          {
            localPosition.z *= _halfsize / absZ;
          }
        }
        particle.position = toField * glm::vec4(localPosition, 1.f);
      };

      field->updateParticles(boxDelegate, typeMask());
      break;
    }

  case CYLINDER_SHAPE:
    {
      auto cylinderDelegate =
      [&](ParticleField::ParticleData& particle)
      {
        glm::vec3 localPosition = fromField * glm::vec4(particle.position, 1.f);
        if (localPosition.z >= _halfsize || localPosition.z <= -_halfsize)
        {
          return;
        }

        glm::vec2 localXY = glm::vec2(localPosition);

        float radius = glm::length(localXY);
        if(radius >= _halfsize) return;

        if (localPosition.z > radius)
        {
          localPosition.z = _halfsize;
        }
        else if (localPosition.z < -radius)
        {
          localPosition.z = -_halfsize;
        }
        else
        {
          localXY *= _halfsize / radius;
          localPosition = glm::vec3(localXY, localPosition.z);
        }

        particle.position = toField * glm::vec4(localPosition, 1.f);
      };

      field->updateParticles(cylinderDelegate, typeMask());
      break;
    }
  }
}
