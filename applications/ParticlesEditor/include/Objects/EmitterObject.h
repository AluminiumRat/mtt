#pragma once

#include <map>
#include <random>

#include <glm/vec3.hpp>

#include <mtt/application/Scene/ObjectLink.h>
#include <mtt/application/TimeT.h>

#include <Objects/HierarhicalObject.h>
#include <Objects/ParticleField.h>

class EmitterObject : public HierarhicalObject
{
  Q_OBJECT
  DEFINE_EXTENSION_ACCEPT(PEVisitorExtension,
                          visitEmitterObject,
                          visitConstEmitterObject,
                          HierarhicalObject)
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

  Q_PROPERTY( bool enabled
              READ enabled
              WRITE setEnabled
              NOTIFY enabledChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( uint32_t typeMask
              READ typeMask
              WRITE setTypeMask
              NOTIFY typeMaskChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( float intensity
              READ size
              WRITE setIntensity
              NOTIFY intensityChanged
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

  Q_PROPERTY( Shape shape
              READ shape
              WRITE setShape
              NOTIFY shapeChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( Distribution distribution
              READ distribution
              WRITE setDistribution
              NOTIFY distributionChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( float directionAngle
              READ directionAngle
              WRITE setDirectionAngle
              NOTIFY directionAngleChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( mtt::Range<float> speedRange
              READ speedRange
              WRITE setSpeedRange
              NOTIFY speedRangeChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( mtt::Range<float> sizeRange
              READ sizeRange
              WRITE setSizeRange
              NOTIFY sizeRangeChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( mtt::Range<float> rotationRange
              READ rotationRange
              WRITE setRotationRange
              NOTIFY rotationRangeChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( mtt::Range<float> rotationSpeedRange
              READ rotationSpeedRange
              WRITE setRotationSpeedRange
              NOTIFY rotationSpeedRangeChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( glm::vec3 firstAlbedo
              READ firstAlbedo
              WRITE setFirstAlbedo
              NOTIFY firstAlbedoChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( glm::vec3 secondAlbedo
              READ secondAlbedo
              WRITE setSecondAlbedo
              NOTIFY secondAlbedoChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( mtt::Range<float> opacityRange
              READ opacityRange
              WRITE setOpacityRange
              NOTIFY opacityRangeChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( glm::vec3 emissionColor
              READ emissionColor
              WRITE setEmissionColor
              NOTIFY emissionColorChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( float emissionBrightness
              READ emissionBrightness
              WRITE setEmissionBrightness
              NOTIFY emissionBrightnessChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( uint8_t textureIndex
              READ textureIndex
              WRITE setTextureIndex
              NOTIFY textureIndexChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( uint8_t tileIndex
              READ tileIndex
              WRITE setTileIndex
              NOTIFY tileIndexChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( mtt::TimeRange lifetimeRange
              READ lifetimeRange
              WRITE setLifetimeRange
              NOTIFY lifetimeRangeChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( mtt::Range<float> massRange
              READ massRange
              WRITE setMassRange
              NOTIFY massRangeChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( mtt::Range<float> frictionFactorRange
              READ frictionFactorRange
              WRITE setFrictionFactorRange
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

  inline bool enabled() const noexcept;
  void setEnabled(bool newValue) noexcept;

  inline uint32_t typeMask() const noexcept;
  void setTypeMask(uint32_t newValue) noexcept;

  inline mtt::ObjectRef<ParticleField>& fieldRef() noexcept;
  inline const mtt::ObjectRef<ParticleField>& fieldRef() const noexcept;

  /// Emitting intensity in particles per second
  inline float intensity() const noexcept;
  void setIntensity(float newValue) noexcept;

  inline float size() const noexcept;
  void setSize(float newValue) noexcept;

  inline Shape shape() const noexcept;
  void setShape(Shape newValue) noexcept;

  inline Distribution distribution() const noexcept;
  void setDistribution(Distribution newValue) noexcept;

  inline float directionAngle() const noexcept;
  void setDirectionAngle(float newValue) noexcept;

  inline const mtt::Range<float>& speedRange() const noexcept;
  void setSpeedRange(const mtt::Range<float>& newValue) noexcept;

  inline const mtt::Range<float>& sizeRange() const noexcept;
  void setSizeRange(const mtt::Range<float>& newValue) noexcept;

  inline const mtt::Range<float>& rotationRange() const noexcept;
  void setRotationRange(const mtt::Range<float>& newValue) noexcept;

  inline const mtt::Range<float>& rotationSpeedRange() const noexcept;
  void setRotationSpeedRange(const mtt::Range<float>& newValue) noexcept;

  inline const glm::vec3& firstAlbedo() const noexcept;
  void setFirstAlbedo(const glm::vec3& newValue) noexcept;

  inline const glm::vec3& secondAlbedo() const noexcept;
  void setSecondAlbedo(const glm::vec3& newValue) noexcept;

  inline const mtt::Range<float>& opacityRange() const noexcept;
  void setOpacityRange(const mtt::Range<float>& newValue) noexcept;

  inline const glm::vec3& emissionColor() const noexcept;
  void setEmissionColor(const glm::vec3& newValue) noexcept;

  inline float emissionBrightness() const noexcept;
  void setEmissionBrightness(float newValue) noexcept;

  inline uint8_t textureIndex() const noexcept;
  void setTextureIndex(uint8_t newValue) noexcept;

  inline uint8_t tileIndex() const noexcept;
  void setTileIndex(uint8_t newValue) noexcept;

  inline const mtt::TimeRange& lifetimeRange() const noexcept;
  void setLifetimeRange(const mtt::TimeRange& newValue) noexcept;

  inline const mtt::Range<float>& massRange() const noexcept;
  void setMassRange(const mtt::Range<float>& newValue) noexcept;

  inline const mtt::Range<float>& frictionFactorRange() const noexcept;
  void setFrictionFactorRange(const mtt::Range<float>& newValue) noexcept;

  virtual void simulationStep(mtt::TimeRange time);
  void emitParticles(float particlesNumber) noexcept;

signals:
  void enabledChanged(bool newValue);
  void typeMaskChanged(uint32_t newValue);
  void fieldChanged(ParticleField * newField);
  void intensityChanged(float newValue);
  void sizeChanged(float newValue);
  void shapeChanged(Shape newValue);
  void distributionChanged(Distribution newValue);
  void directionAngleChanged(float newValue);
  void speedRangeChanged(const mtt::Range<float>& newValue);
  void sizeRangeChanged(const mtt::Range<float>& newValue);
  void rotationRangeChanged(const mtt::Range<float>& newValue);
  void rotationSpeedRangeChanged(const mtt::Range<float>& newValue);
  void firstAlbedoChanged(const glm::vec3& newValue);
  void secondAlbedoChanged(const glm::vec3& newValue);
  void opacityRangeChanged(const mtt::Range<float>& newValue);
  void emissionColorChanged(const glm::vec3& newValue);
  void emissionBrightnessChanged(float newValue);
  void textureIndexChanged(uint8_t newValue);
  void tileIndexChanged(uint8_t newValue);
  void lifetimeRangeChanged(const mtt::TimeRange& newValue);
  void massRangeChanged(const mtt::Range<float>& newValue);
  void frictionFactorRangeChanged(const mtt::Range<float>& newValue);

private:
  void _connectToField(ParticleField& field);
  void _disconnectFromField(ParticleField& field) noexcept;
  glm::vec4 _getParticlePosition() const noexcept;
  glm::vec4 _getParticleSpeed() const noexcept;

private:
  bool _enabled;
  uint32_t _typeMask;
  mtt::ObjectLink<ParticleField,
                  EmitterObject,
                  &EmitterObject::_connectToField,
                  &EmitterObject::_disconnectFromField,
                  &EmitterObject::fieldChanged> _fieldRef;

  float _intensity;
  float _size;
  Shape _shape;
  Distribution _distribution;

  float _directionAngle;
  mtt::Range<float> _speedRange;
  mtt::Range<float> _sizeRange;
  mtt::Range<float> _rotationRange;
  mtt::Range<float> _rotationSpeedRange;
  glm::vec3 _firstAlbedo;
  glm::vec3 _secondAlbedo;
  mtt::Range<float> _opacityRange;
  glm::vec3 _emissionColor;
  float _emissionBrightness;
  uint8_t _textureIndex;
  uint8_t _tileIndex;
  mtt::TimeRange _lifetimeRange;
  mtt::Range<float> _massRange;
  mtt::Range<float> _frictionFactorRange;

  mutable std::default_random_engine _randomEngine;
  std::uniform_real_distribution<float> _symmetricalDistribution;
  std::uniform_real_distribution<float> _displacedDistribution;
};

inline bool EmitterObject::enabled() const noexcept
{
  return _enabled;
}

inline uint32_t EmitterObject::typeMask() const noexcept
{
  return _typeMask;
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

inline float EmitterObject::intensity() const noexcept
{
  return _intensity;
}

inline float EmitterObject::size() const noexcept
{
  return _size;
}

inline EmitterObject::Shape EmitterObject::shape() const noexcept
{
  return _shape;
}

inline EmitterObject::Distribution EmitterObject::distribution() const noexcept
{
  return _distribution;
}

inline float EmitterObject::directionAngle() const noexcept
{
  return _directionAngle;
}

inline const mtt::Range<float>& EmitterObject::speedRange() const noexcept
{
  return _speedRange;
}

inline const mtt::Range<float>& EmitterObject::sizeRange() const noexcept
{
  return _sizeRange;
}

inline const mtt::Range<float>& EmitterObject::rotationRange() const noexcept
{
  return _rotationRange;
}

inline const mtt::Range<float>&
                              EmitterObject::rotationSpeedRange() const noexcept
{
  return _rotationSpeedRange;
}

inline const glm::vec3& EmitterObject::firstAlbedo() const noexcept
{
  return _firstAlbedo;
}

inline const glm::vec3& EmitterObject::secondAlbedo() const noexcept
{
  return _secondAlbedo;
}

inline const mtt::Range<float>& EmitterObject::opacityRange() const noexcept
{
  return _opacityRange;
}

inline const glm::vec3& EmitterObject::emissionColor() const noexcept
{
  return _emissionColor;
}

inline float EmitterObject::emissionBrightness() const noexcept
{
  return _emissionBrightness;
}

inline uint8_t EmitterObject::textureIndex() const noexcept
{
  return _textureIndex;
}

inline uint8_t EmitterObject::tileIndex() const noexcept
{
  return _tileIndex;
}

inline const mtt::TimeRange& EmitterObject::lifetimeRange() const noexcept
{
  return _lifetimeRange;
}

inline const mtt::Range<float>& EmitterObject::massRange() const noexcept
{
  return _massRange;
}

inline const mtt::Range<float>&
                            EmitterObject::frictionFactorRange() const noexcept
{
  return _frictionFactorRange;
}
