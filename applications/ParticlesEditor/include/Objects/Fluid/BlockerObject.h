#pragma once

#include <Objects/ModificatorObject.h>

class FluidObject;

class BlockerObject : public ModificatorObject
{
  Q_OBJECT

  DEFINE_EXTENSION_ACCEPT(PEVisitorExtension,
                          visitBlockerObject,
                          visitConstBlockerObject,
                          ModificatorObject)

public:
  enum Shape
  {
    SPHERE_SHAPE,
    BOX_SHAPE,
    CYLINDER_SHAPE
  };
  const static std::map<Shape, QString> shapeNames;

  Q_PROPERTY( Shape shape
              READ shape
              WRITE setShape
              NOTIFY shapeChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( float size
              READ size
              WRITE setSize
              NOTIFY sizeChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

public:
  BlockerObject(const QString& name,
                bool canBeRenamed,
                const mtt::UID& id = mtt::UID());
  BlockerObject(const BlockerObject&) = delete;
  BlockerObject& operator = (const BlockerObject&) = delete;
  virtual ~BlockerObject() noexcept;

  inline Shape shape() const noexcept;
  void setShape(Shape newValue) noexcept;

  inline float size() const noexcept;
  void setSize(float newValue) noexcept;

  bool isPointInside(const glm::vec3& point) noexcept;

  virtual void simulationStep(mtt::TimeRange time) override;

signals:
  void shapeChanged(Shape newValue);
  void sizeChanged(float newValue);

protected:
  virtual void connectToField(ParticleField& field) override;
  virtual void disconnectFromField(ParticleField& field) noexcept override;

private:
  void _connectToFluid(FluidObject& fluid);
  void _disconnectFromFluid(FluidObject& fluid) noexcept;

private:
  Shape _shape;
  float _size;
  float _halfsize;
};

inline BlockerObject::Shape BlockerObject::shape() const noexcept
{
  return _shape;
}

inline float BlockerObject::size() const noexcept
{
  return _size;
}
