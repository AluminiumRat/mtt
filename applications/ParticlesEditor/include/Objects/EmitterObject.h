#pragma once

#include <map>

#include <Objects/ModificatorObject.h>

class EmitterObject : public ModificatorObject
{
  Q_OBJECT
  DEFINE_EXTENSION_ACCEPT(PEVisitorExtension,
                          visitEmitterObject,
                          visitConstEmitterObject,
                          ModificatorObject)
public:
  enum Shape
  {
    CIRCLE_SHAPE = 0,
    SPHERE_SHAPE = 1
  };
  const static std::map<Shape, QString> shapeNames;

  enum Distribution
  {
    UNIFORM_DISTRIBUTION = 0,
    SMOOTH_DISTRIBUTION = 1
  };
  const static std::map<Distribution, QString> distributionNames;

  Q_PROPERTY( float intensity
              READ size
              WRITE setIntensity
              RESET resetIntensity
              NOTIFY intensityChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( float size
              READ size
              WRITE setSize
              RESET resetSize
              NOTIFY sizeChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( Shape shape
              READ shape
              WRITE setShape
              RESET resetShape
              NOTIFY shapeChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( Distribution distribution
              READ distribution
              WRITE setDistribution
              RESET resetDistribution
              NOTIFY distributionChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

public:
  EmitterObject(const QString& name,
                bool canBeRenamed,
                const mtt::UID& id = mtt::UID());
  EmitterObject(const EmitterObject&) = delete;
  EmitterObject& operator = (const EmitterObject&) = delete;
  virtual ~EmitterObject() noexcept = default;

  /// Emitting intensity in particles per second
  inline float intensity() const noexcept;
  void setIntensity(float newValue) noexcept;
  inline void resetIntensity() noexcept;

  inline float size() const noexcept;
  void setSize(float newValue) noexcept;
  inline void resetSize() noexcept;

  inline Shape shape() const noexcept;
  void setShape(Shape newValue) noexcept;
  inline void resetShape() noexcept;

  inline Distribution distribution() const noexcept;
  void setDistribution(Distribution newValue) noexcept;
  inline void resetDistribution() noexcept;

signals:
  void intensityChanged(float newValue);
  void sizeChanged(float newValue);
  void shapeChanged(Shape newValue);
  void distributionChanged(Distribution newValue);

private:
  float _intensity;
  float _size;
  Shape _shape;
  Distribution _distribution;
};

inline float EmitterObject::intensity() const noexcept
{
  return _intensity;
}

inline void EmitterObject::resetIntensity() noexcept
{
  setIntensity(0.f);
}

inline float EmitterObject::size() const noexcept
{
  return _size;
}

inline void EmitterObject::resetSize() noexcept
{
  setSize(1.f);
}

inline EmitterObject::Shape EmitterObject::shape() const noexcept
{
  return _shape;
}

inline void EmitterObject::resetShape() noexcept
{
  setShape(SPHERE_SHAPE);
}

inline EmitterObject::Distribution EmitterObject::distribution() const noexcept
{
  return _distribution;
}

inline void EmitterObject::resetDistribution() noexcept
{
  setDistribution(UNIFORM_DISTRIBUTION);
}
