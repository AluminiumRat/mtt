#include <chrono>

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
  _distribution(UNIFORM_DISTRIBUTION),
  _directionAngle(2.f * glm::pi<float>()),
  _sizeRange(1.f, 1.f),
  _rotationRange(-glm::pi<float>(), glm::pi<float>()),
  _rotationSpeedRange(.0f, .0f),
  _firstColor(1.f, 1.f, 1.f),
  _secondColor(1.f, 1.f, 1.f),
  _opacityRange(1.f, 1.f),
  _ignoreBrightness(true),
  _brightnessRange(1.f, 1.f),
  _textureIndex(0),
  _lifetimeRange(
              std::chrono::duration_cast<mtt::TimeT>(std::chrono::seconds(1)),
              std::chrono::duration_cast<mtt::TimeT>(std::chrono::seconds(1))),
  _massRange(0.f, 0.f),
  _frictionFactorRange(0.f, 0.f),
  _randomEngine(
    unsigned int(std::chrono::system_clock::now().time_since_epoch().count())),
  _symmetricalDistribution(-1.f, 1.f),
  _displacedDistribution(0.f, 1.f)
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

void EmitterObject::setDirectionAngle(float newValue) noexcept
{
  if(_directionAngle == newValue) return;
  _directionAngle = newValue;
  emit directionAngleChanged(newValue);
}

void EmitterObject::setSpeedRange(const mtt::Range<float>& newValue) noexcept
{
  if(_speedRange == newValue) return;
  _speedRange = newValue;
  emit speedRangeChanged(newValue);
}

void EmitterObject::setSizeRange(const mtt::Range<float>& newValue) noexcept
{
  if(_sizeRange == newValue) return;
  _sizeRange = newValue;
  emit sizeRangeChanged(newValue);
}

void EmitterObject::setRotationRange(const mtt::Range<float>& newValue) noexcept
{
  if(_rotationRange == newValue) return;
  _rotationRange = newValue;
  emit rotationRangeChanged(newValue);
}

void EmitterObject::setRotationSpeedRange(
                                    const mtt::Range<float>& newValue) noexcept
{
  if(_rotationSpeedRange == newValue) return;
  _rotationSpeedRange = newValue;
  emit rotationSpeedRangeChanged(newValue);
}

void EmitterObject::setFirstColor(const glm::vec3& newValue) noexcept
{
  if(_firstColor == newValue) return;
  _firstColor = newValue;
  emit firstColorChanged(newValue);
}

void EmitterObject::setSecondColor(const glm::vec3& newValue) noexcept
{
  if(_secondColor == newValue) return;
  _secondColor = newValue;
  emit secondColorChanged(newValue);
}

void EmitterObject::setOpacityRange(const mtt::Range<float>& newValue) noexcept
{
  if(_opacityRange == newValue) return;
  _opacityRange = newValue;
  emit opacityRangeChanged(newValue);
}

void EmitterObject::setIgnoreBrightness(bool newValue) noexcept
{
  if(_ignoreBrightness == newValue) return;
  _ignoreBrightness = newValue;
  emit ignoreBrightnessChanged(newValue);
}

void EmitterObject::setBrightnessRange(
                                    const mtt::Range<float>& newValue) noexcept
{
  if(_brightnessRange == newValue) return;
  _brightnessRange = newValue;
  emit brightnessRangeChanged(newValue);
}

void EmitterObject::setTextureIndex(uint8_t newValue) noexcept
{
  if(_textureIndex == newValue) return;
  _textureIndex = newValue;
  emit textureIndexChanged(newValue);
}

void EmitterObject::setLifetimeRange(
                            const mtt::Range<mtt::TimeT>& newValue) noexcept
{
  if(_lifetimeRange == newValue) return;
  _lifetimeRange = newValue;
  emit lifetimeRangeChanged(newValue);
}

void EmitterObject::setMassRange(const mtt::Range<float>& newValue) noexcept
{
  if(_massRange == newValue) return;
  _massRange = newValue;
  emit massRangeChanged(newValue);
}

void EmitterObject::setFrictionFactorRange(
                                    const mtt::Range<float>& newValue) noexcept
{
  if(_frictionFactorRange == newValue) return;
  _frictionFactorRange = newValue;
  emit frictionFactorRangeChanged(newValue);
}

void EmitterObject::simulationStep(mtt::TimeT currentTime, mtt::TimeT delta)
{
  if(_intensity <= 0.f) return;

  using FloatTime = std::chrono::duration<float>;
  float floatDelta = std::chrono::duration_cast<FloatTime>(delta).count();

  float floatParticlesNumber = _intensity * floatDelta;
  size_t particlesNumber = size_t(floatParticlesNumber);

  float remainder = floatParticlesNumber - particlesNumber;
  if(_displacedDistribution(_randomEngine) < remainder) particlesNumber++;

  emitParticles(particlesNumber);
}

glm::vec4 EmitterObject::_getParticlePosition() const noexcept
{
  if(_shape == CIRCLE_SHAPE)
  {
    float angle = glm::pi<float>() * _symmetricalDistribution(_randomEngine);
    float radius = sqrt(_displacedDistribution(_randomEngine));
    if (_distribution == SMOOTH_DISTRIBUTION)
    {
      radius = 1.f - sqrt(1.f - radius);
    }
    radius *= _size / 2.f;
    return glm::vec4( sin(angle) * radius,
                      cos(angle) * radius,
                      0.f,
                      1.f);
  }
  else
  {
    float angle1 = glm::pi<float>() * _symmetricalDistribution(_randomEngine);
    float height = _symmetricalDistribution(_randomEngine);
    float angle2 = asin(height);

    float radius = pow(_displacedDistribution(_randomEngine), 1.f / 3.f);
    if (_distribution == SMOOTH_DISTRIBUTION)
    {
      radius = 1.f - sqrt(1.f - radius);
    }
    radius *= _size / 2.f;

    return glm::vec4( sin(angle1) * cos(angle2) * radius,
                      cos(angle1) * cos(angle2) * radius,
                      height * radius,
                      1.f);
  }
}

glm::vec4 EmitterObject::_getParticleSpeed() const noexcept
{
  float angle1 = glm::pi<float>() * _symmetricalDistribution(_randomEngine);
  float startZ = cos(_directionAngle / 2.f);
  float zValue = glm::mix(startZ, 1.f, _displacedDistribution(_randomEngine));
  float angle2 = asin(zValue);
  float length = glm::mix(_speedRange.min(),
                          _speedRange.max(),
                          _displacedDistribution(_randomEngine));
  return glm::vec4( sin(angle1) * cos(angle2) * length,
                    cos(angle1) * cos(angle2) * length,
                    zValue * length,
                    0.f);
}

void EmitterObject::emitParticles(size_t particlesNumber) noexcept
{
  if(!enabled()) return;
  if(particlesNumber == 0) return;

  ParticleField* field = fieldRef().get();
  if(field == nullptr) return;

  glm::mat4x4 toField = glm::inverse(field->localToWorldTransform()) *
                                                        localToWorldTransform();

  std::uniform_int_distribution<mtt::TimeT::rep>
                  timeDistribution( glm::max(_lifetimeRange.min().count(), 0),
                                    glm::max(_lifetimeRange.max().count(), 0));

  for (size_t i = 0; i < particlesNumber; i++)
  {
    ParticleField::ParticleData newParticle;
    newParticle.typeMask = typeMask();
    newParticle.position = toField * _getParticlePosition();
    newParticle.speed = toField * _getParticleSpeed();
    newParticle.size = glm::mix(_sizeRange.min(),
                                _sizeRange.max(),
                                _displacedDistribution(_randomEngine));
    newParticle.size = glm::max(newParticle.size, 0.f);
    newParticle.rotation = glm::mix(_rotationRange.min(),
                                    _rotationRange.max(),
                                    _displacedDistribution(_randomEngine));
    newParticle.rotationSpeed = glm::mix(
                                      _rotationSpeedRange.min(),
                                      _rotationSpeedRange.max(),
                                      _displacedDistribution(_randomEngine));
    newParticle.color = glm::mix( _firstColor,
                                  _secondColor,
                                  _displacedDistribution(_randomEngine));
    newParticle.color = glm::clamp( newParticle.color,
                                    glm::vec3(0.f),
                                    glm::vec3(1.f));
    newParticle.opacity = glm::mix( _opacityRange.min(),
                                    _opacityRange.max(),
                                   _displacedDistribution(_randomEngine));
    if (_ignoreBrightness) newParticle.brightness = newParticle.opacity;
    else
    {
      newParticle.brightness = glm::mix(_brightnessRange.min(),
                                        _brightnessRange.max(),
                                        _displacedDistribution(_randomEngine));
      newParticle.brightness = glm::max(newParticle.brightness, 0.f);
    }
    newParticle.textureIndex = _textureIndex;
    newParticle.currentTime = mtt::TimeT(0);
    newParticle.maxTime = mtt::TimeT(timeDistribution(_randomEngine));
    newParticle.mass = glm::mix(_massRange.min(),
                                _massRange.max(),
                                _displacedDistribution(_randomEngine));
    newParticle.mass = glm::max(0.f, newParticle.mass);

    newParticle.frictionFactor = glm::mix(
                                        _frictionFactorRange.min(),
                                        _frictionFactorRange.max(),
                                        _displacedDistribution(_randomEngine));
    newParticle.frictionFactor = glm::max(0.f, newParticle.frictionFactor);

    field->addParticle(newParticle);
  }
}
