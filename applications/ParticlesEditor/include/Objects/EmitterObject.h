#pragma once

#include <map>
#include <random>

#include <glm/vec3.hpp>

#include <mtt/application/Scene/ObjectLink.h>
#include <mtt/application/Application.h>
#include <mtt/utilities/Range.h>

#include <Objects/ModificatorObject.h>
#include <Objects/ParticleField.h>

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

  using TimeType = mtt::Application::TimeType;

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

  Q_PROPERTY( float directionAngle
              READ directionAngle
              WRITE setDirectionAngle
              RESET resetDirectionAngle
              NOTIFY directionAngleChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( mtt::Range<float> speedRange
              READ speedRange
              WRITE setSpeedRange
              RESET resetSpeedRange
              NOTIFY speedRangeChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( uint32_t typeMask
              READ typeMask
              WRITE setTypeMask
              RESET resetTypeMask
              NOTIFY typeMaskChanged
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

  inline float directionAngle() const noexcept;
  void setDirectionAngle(float newValue) noexcept;
  inline void resetDirectionAngle() noexcept;

  inline const mtt::Range<float>& speedRange() const noexcept;
  void setSpeedRange(const mtt::Range<float>& newValue) noexcept;
  inline void resetSpeedRange() noexcept;

  inline uint32_t typeMask() const noexcept;
  void setTypeMask(uint32_t newValue) noexcept;
  inline void resetTypeMask() noexcept;

  inline mtt::ObjectRef<ParticleField>& fieldRef() noexcept;
  inline const mtt::ObjectRef<ParticleField>& fieldRef() const noexcept;

  void simulationStep(TimeType currentTime, TimeType delta);
  void emitParticles(size_t particlesNumber) noexcept;

signals:
  void intensityChanged(float newValue);
  void sizeChanged(float newValue);
  void shapeChanged(Shape newValue);
  void distributionChanged(Distribution newValue);
  void directionAngleChanged(float newValue);
  void speedRangeChanged(const mtt::Range<float>& newValue);
  void typeMaskChanged(uint32_t newValue);

private:
  glm::vec4 _getParticlePosition() const noexcept;
  glm::vec4 _getParticleSpeed() const noexcept;

private:
  float _intensity;
  float _size;
  Shape _shape;
  Distribution _distribution;

  float _directionAngle;
  mtt::Range<float> _speedRange;
  uint32_t _typeMask;

  mtt::ObjectLink<ParticleField, EmitterObject, nullptr, nullptr> _fieldRef;

  mutable std::default_random_engine _randomEngine;
  std::uniform_real_distribution<float> _symmetricalDistribution;
  std::uniform_real_distribution<float> _displacedDistribution;
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

inline float EmitterObject::directionAngle() const noexcept
{
  return _directionAngle;
}

inline void EmitterObject::resetDirectionAngle() noexcept
{
  setDirectionAngle(2.f * glm::pi<float>());
}

inline const mtt::Range<float>& EmitterObject::speedRange() const noexcept
{
  return _speedRange;
}

inline void EmitterObject::resetSpeedRange() noexcept
{
  setSpeedRange(mtt::Range<float>());
}

inline uint32_t EmitterObject::typeMask() const noexcept
{
  return _typeMask;
}

inline void EmitterObject::resetTypeMask() noexcept
{
  setTypeMask(1);
}

inline mtt::ObjectRef<ParticleField>& EmitterObject::fieldRef() noexcept
{
  return _fieldRef;
}

inline const mtt::ObjectRef<ParticleField>&
                                        EmitterObject::fieldRef() const noexcept
{
  return _fieldRef;
}
