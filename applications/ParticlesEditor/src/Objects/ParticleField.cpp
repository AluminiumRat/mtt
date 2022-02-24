#include <Objects/ParticleField.h>

ParticleField::ParticleField( const QString& name,
                              bool canBeRenamed,
                              const mtt::UID& id) :
  MovableObject(name, canBeRenamed, id),
  _size(10.f, 10.f, 10.f)
{
}

void ParticleField::setSize(const glm::vec3& newValue)
{
  if(_size == newValue) return;
  _size = newValue;
  emit sizeChanged(_size);
}
