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
  _fieldRef(*this),
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

void EmitterObject::simulationStep(TimeType currentTime, TimeType delta)
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

void EmitterObject::emitParticles(size_t particlesNumber) noexcept
{
  if(particlesNumber == 0) return;

  ParticleField* field = _fieldRef.get();
  if(field == nullptr) return;

  glm::mat4x4 toField = glm::inverse(field->localToWorldTransform()) *
                                                        localToWorldTransform();

  std::uniform_int_distribution<int> timeDistribution(300, 5000);

  for (size_t i = 0; i < particlesNumber; i++)
  {
    ParticleField::ParticleData newParticle;
    newParticle.typeIndex = 0;
    newParticle.position = toField * _getParticlePosition();
    newParticle.speed = glm::vec3(_symmetricalDistribution(_randomEngine),
                                  _symmetricalDistribution(_randomEngine),
                                  _symmetricalDistribution(_randomEngine));
    newParticle.size = _displacedDistribution(_randomEngine) + .1f;
    newParticle.rotation = _symmetricalDistribution(_randomEngine);
    newParticle.rotationSpeed = _symmetricalDistribution(_randomEngine);
    newParticle.color = glm::vec3(_displacedDistribution(_randomEngine),
                                  _displacedDistribution(_randomEngine),
                                  _displacedDistribution(_randomEngine));
    newParticle.brightness = .5f + .5f * _displacedDistribution(_randomEngine);
    newParticle.opacity = newParticle.brightness;
    newParticle.textureIndex = 0;
    if(i > 15) newParticle.textureIndex = 1;
    newParticle.currentTime = ParticleField::TimeType(0);
    newParticle.maxTime =
                      ParticleField::TimeType(timeDistribution(_randomEngine));

    field->addParticle(newParticle);
  }
}
