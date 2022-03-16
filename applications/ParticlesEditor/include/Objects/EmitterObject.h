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

  Q_PROPERTY( mtt::Range<float> sizeRange
              READ sizeRange
              WRITE setSizeRange
              RESET resetSizeRange
              NOTIFY sizeRangeChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( mtt::Range<float> rotationRange
              READ rotationRange
              WRITE setRotationRange
              RESET resetRotationRange
              NOTIFY rotationRangeChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( mtt::Range<float> rotationSpeedRange
              READ rotationSpeedRange
              WRITE setRotationSpeedRange
              RESET resetRotationSpeedRange
              NOTIFY rotationSpeedRangeChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( glm::vec3 firstColor
              READ firstColor
              WRITE setFirstColor
              RESET resetFirstColor
              NOTIFY firstColorChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( glm::vec3 secondColor
              READ secondColor
              WRITE setSecondColor
              RESET resetSecondColor
              NOTIFY secondColorChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( mtt::Range<float> opacityRange
              READ opacityRange
              WRITE setOpacityRange
              RESET resetOpacityRange
              NOTIFY opacityRangeChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( bool ignoreBrightness
              READ ignoreBrightness
              WRITE setIgnoreBrightness
              RESET resetIgnoreBrightness
              NOTIFY ignoreBrightnessChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( mtt::Range<float> brightnessRange
              READ brightnessRange
              WRITE setBrightnessRange
              RESET resetBrightnessRange
              NOTIFY brightnessRangeChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( uint8_t textureIndex
              READ textureIndex
              WRITE setTextureIndex
              RESET resetTextureIndex
              NOTIFY textureIndexChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( mtt::Range<TimeType> lifetimeRange
              READ lifetimeRange
              WRITE setLifetimeRange
              RESET resetLifetimeRange
              NOTIFY lifetimeRangeChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( mtt::Range<float> massRange
              READ massRange
              WRITE setMassRange
              RESET resetMassRange
              NOTIFY massRangeChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( mtt::Range<float> frictionFactorRange
              READ frictionFactorRange
              WRITE setFrictionFactorRange
              RESET resetFrictionFactorRange
              NOTIFY frictionFactorRangeChanged
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

  inline const mtt::Range<float>& sizeRange() const noexcept;
  void setSizeRange(const mtt::Range<float>& newValue) noexcept;
  inline void resetSizeRange() noexcept;
  inline const mtt::Range<float>& rotationRange() const noexcept;
  void setRotationRange(const mtt::Range<float>& newValue) noexcept;
  inline void resetRotationRange() noexcept;
  inline const mtt::Range<float>& rotationSpeedRange() const noexcept;
  void setRotationSpeedRange(const mtt::Range<float>& newValue) noexcept;
  inline void resetRotationSpeedRange() noexcept;
  inline const glm::vec3& firstColor() const noexcept;
  void setFirstColor(const glm::vec3& newValue) noexcept;
  inline void resetFirstColor() noexcept;
  inline const glm::vec3& secondColor() const noexcept;
  void setSecondColor(const glm::vec3& newValue) noexcept;
  inline void resetSecondColor() noexcept;
  inline const mtt::Range<float>& opacityRange() const noexcept;
  void setOpacityRange(const mtt::Range<float>& newValue) noexcept;
  inline void resetOpacityRange() noexcept;
  inline bool ignoreBrightness() const noexcept;
  void setIgnoreBrightness(bool newValue) noexcept;
  inline void resetIgnoreBrightness() noexcept;
  inline const mtt::Range<float>& brightnessRange() const noexcept;
  void setBrightnessRange(const mtt::Range<float>& newValue) noexcept;
  inline void resetBrightnessRange() noexcept;
  inline uint8_t textureIndex() const noexcept;
  void setTextureIndex(uint8_t newValue) noexcept;
  inline void resetTextureIndex() noexcept;
  inline const mtt::Range<TimeType>& lifetimeRange() const noexcept;
  void setLifetimeRange(const mtt::Range<TimeType>& newValue) noexcept;
  inline void resetLifetimeRange() noexcept;
  inline const mtt::Range<float>& massRange() const noexcept;
  void setMassRange(const mtt::Range<float>& newValue) noexcept;
  inline void resetMassRange() noexcept;
  inline const mtt::Range<float>& frictionFactorRange() const noexcept;
  void setFrictionFactorRange(const mtt::Range<float>& newValue) noexcept;
  inline void resetFrictionFactorRange() noexcept;

  virtual void simulationStep(TimeType currentTime, TimeType delta) override;
  void emitParticles(size_t particlesNumber) noexcept;

signals:
  void intensityChanged(float newValue);
  void sizeChanged(float newValue);
  void shapeChanged(Shape newValue);
  void distributionChanged(Distribution newValue);
  void directionAngleChanged(float newValue);
  void speedRangeChanged(const mtt::Range<float>& newValue);
  void sizeRangeChanged(const mtt::Range<float>& newValue);
  void rotationRangeChanged(const mtt::Range<float>& newValue);
  void rotationSpeedRangeChanged(const mtt::Range<float>& newValue);
  void firstColorChanged(const glm::vec3& newValue);
  void secondColorChanged(const glm::vec3& newValue);
  void opacityRangeChanged(const mtt::Range<float>& newValue);
  void ignoreBrightnessChanged(bool newValue);
  void brightnessRangeChanged(const mtt::Range<float>& newValue);
  void textureIndexChanged(uint8_t newValue);
  void lifetimeRangeChanged(const mtt::Range<TimeType>& newValue);
  void massRangeChanged(const mtt::Range<float>& newValue);
  void frictionFactorRangeChanged(const mtt::Range<float>& newValue);

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
  mtt::Range<float> _sizeRange;
  mtt::Range<float> _rotationRange;
  mtt::Range<float> _rotationSpeedRange;
  glm::vec3 _firstColor;
  glm::vec3 _secondColor;
  mtt::Range<float> _opacityRange;
  bool _ignoreBrightness;
  mtt::Range<float> _brightnessRange;
  uint8_t _textureIndex;
  mtt::Range<TimeType> _lifetimeRange;
  mtt::Range<float> _massRange;
  mtt::Range<float> _frictionFactorRange;

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

inline const mtt::Range<float>& EmitterObject::sizeRange() const noexcept
{
  return _sizeRange;
}

inline void EmitterObject::resetSizeRange() noexcept
{
  setSizeRange(mtt::Range<float>(1.f, 1.f));
}

inline const mtt::Range<float>& EmitterObject::rotationRange() const noexcept
{
  return _rotationRange;
}

inline void EmitterObject::resetRotationRange() noexcept
{
  setRotationRange(mtt::Range<float>(-glm::pi<float>(), glm::pi<float>()));
}

inline const mtt::Range<float>&
                              EmitterObject::rotationSpeedRange() const noexcept
{
  return _rotationSpeedRange;
}

inline void EmitterObject::resetRotationSpeedRange() noexcept
{
  setRotationSpeedRange(mtt::Range<float>(0.f, 0.f));
}

inline const glm::vec3& EmitterObject::firstColor() const noexcept
{
  return _firstColor;
}

inline void EmitterObject::resetFirstColor() noexcept
{
  setFirstColor(glm::vec3(1.f, 1.f, 1.f));
}

inline const glm::vec3& EmitterObject::secondColor() const noexcept
{
  return _secondColor;
}

inline void EmitterObject::resetSecondColor() noexcept
{
  setSecondColor(glm::vec3(1.f, 1.f, 1.f));
}

inline const mtt::Range<float>& EmitterObject::opacityRange() const noexcept
{
  return _opacityRange;
}

inline void EmitterObject::resetOpacityRange() noexcept
{
  setOpacityRange(mtt::Range<float>(1.f, 1.f));
}

inline bool EmitterObject::ignoreBrightness() const noexcept
{
  return _ignoreBrightness;
}

inline void EmitterObject::resetIgnoreBrightness() noexcept
{
  setIgnoreBrightness(true);
}

inline const mtt::Range<float>& EmitterObject::brightnessRange() const noexcept
{
  return _brightnessRange;
}

inline void EmitterObject::resetBrightnessRange() noexcept
{
  setBrightnessRange(mtt::Range<float>(1.f, 1.f));
}

inline uint8_t EmitterObject::textureIndex() const noexcept
{
  return _textureIndex;
}

inline void EmitterObject::resetTextureIndex() noexcept
{
  setTextureIndex(0);
}

inline const mtt::Range<EmitterObject::TimeType>&
                                  EmitterObject::lifetimeRange() const noexcept
{
  return _lifetimeRange;
}

inline void EmitterObject::resetLifetimeRange() noexcept
{
  setLifetimeRange(mtt::Range<TimeType>(
                std::chrono::duration_cast<TimeType>(std::chrono::seconds(1)),
                std::chrono::duration_cast<TimeType>(std::chrono::seconds(1))));
}

inline const mtt::Range<float>& EmitterObject::massRange() const noexcept
{
  return _massRange;
}

inline void EmitterObject::resetMassRange() noexcept
{
  setMassRange(mtt::Range<float>(0.f, 0.f));
}

inline const mtt::Range<float>&
                            EmitterObject::frictionFactorRange() const noexcept
{
  return _frictionFactorRange;
}

inline void EmitterObject::resetFrictionFactorRange() noexcept
{
  setFrictionFactorRange(mtt::Range<float>(0.f, 0.f));
}
