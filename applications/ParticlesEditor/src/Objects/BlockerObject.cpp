#include <Objects/BlockerObject.h>

const std::map<BlockerObject::Shape, QString> BlockerObject::shapeNames =
  { {BlockerObject::SPHERE_SHAPE, QT_TR_NOOP("Sphere")},
    {BlockerObject::BOX_SHAPE, QT_TR_NOOP("Box")},
    {BlockerObject::CYLINDER_SHAPE, QT_TR_NOOP("Cylinder")}};

BlockerObject::BlockerObject( const QString& name,
                              bool canBeRenamed,
                              const mtt::UID& id) :
  ModificatorObject(name, canBeRenamed, id),
  _shape(SPHERE_SHAPE),
  _size(1.f)
{
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
  emit sizeChanged(newValue);
}
