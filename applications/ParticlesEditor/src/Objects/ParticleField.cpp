#include <algorithm>
#include <exception>

#include <mtt/utilities/Abort.h>

#include <Objects/Fluid/FluidObject.h>
#include <Objects/EmitterObject.h>
#include <Objects/ModificatorObject.h>
#include <Objects/ParticleField.h>

ParticleField::ParticleField( const QString& name,
                              bool canBeRenamed,
                              const mtt::UID& theId) :
  MovableObject(name, canBeRenamed, theId),
  _size(10.f, 10.f, 10.f)
{
  mtt::UID fluidId(id().mixedUID(18395279348825422041ull));
  std::unique_ptr<FluidObject> fluid( new FluidObject(tr("Fluid"),
                                                      false,
                                                      *this,
                                                      fluidId));
  _fluid = fluid.get();
  addSubobject(std::move(fluid));
}

void ParticleField::setSize(const glm::vec3& newValue)
{
  if(_size == newValue) return;
  _size = newValue;
  clear();
  emit sizeChanged(_size);
}

void ParticleField::clear() noexcept
{
  _workIndices.clear();
  _freeIndices.clear();
  _particlesData.clear();
  _newParticles.clear();
  _fluid->clear();

  emit cleared();
}

void ParticleField::addParticle(const ParticleData& particle)
{
  _newParticles.push_back(particle);
}

void ParticleField::setTextureDescriptions(
                            const ParticleTextureDescriptions& newDescriptions)
{
  _textureDescriptions.clear();
  try
  {
    _textureDescriptions = newDescriptions;
  }
  catch (...)
  {
    emit textureDescriptionsChanged(_textureDescriptions);
    throw;
  }

  emit textureDescriptionsChanged(_textureDescriptions);
}

void ParticleField::registerEmitter(EmitterObject& emitter)
{
  if(std::find( _emitters.begin(),
                _emitters.end(),
                &emitter) != _emitters.end()) mtt::Abort("ParticleField::registerEmitter: emitter is already registerd.");
  _emitters.push_back(&emitter);
}

void ParticleField::unregisterEmitter(EmitterObject& emitter) noexcept
{
  Emitters::iterator iEmitter = std::find(_emitters.begin(),
                                          _emitters.end(),
                                          &emitter);
  if(iEmitter == _emitters.end()) return;
  _emitters.erase(iEmitter);
}

void ParticleField::registerModificator(ModificatorObject& modificator)
{
  if(std::find( _modificators.begin(),
                _modificators.end(),
                &modificator) != _modificators.end()) mtt::Abort("ParticleField::registerModificator: modificator is already registerd.");
  _modificators.push_back(&modificator);
}

void ParticleField::unregisterModificator(
                                        ModificatorObject& modificator) noexcept
{
  Modificators::iterator iModificator = std::find(_modificators.begin(),
                                                  _modificators.end(),
                                                  &modificator);
  if(iModificator == _modificators.end()) return;
  _modificators.erase(iModificator);
}

void ParticleField::_applyModificators( mtt::TimeRange time,
                                        ModificatorApplyTime applyTime)
{
  for (ModificatorObject* modificator : _modificators)
  {
    if(modificator->enabled() && modificator->applyTime() == applyTime)
    {
      modificator->simulationStep(time);
    }
  }
}

void ParticleField::simulationStep(mtt::TimeRange time)
{
  emit simulationStepStarted();

  try
  {
    for (EmitterObject* emitter : _emitters)
    {
      if(emitter->enabled()) emitter->simulationStep(time);
    }

    _addParticles();

    _applyModificators(time, PREFLUID_TIME);

    _fluid->simulationStep(time);

    _applyModificators(time, POSTFLUID_TIME);

    _updateParticlesData(time.length());

    _applyModificators(time, POSTUPDATE_TIME);

    _deleteParticles();
  }
  catch (...)
  {
    mtt::Log() << "ParticleField::simulationStep: error in simulation, field will be cleared.";
    clear();
    emit simulationStepFinished();
    throw;
  }

  emit simulationStepFinished();
}

void ParticleField::_addParticles()
{
  if(_newParticles.empty()) return;
  std::vector<ParticleIndex> newIndices;

  newIndices.reserve(_newParticles.size());

  if(_newParticles.size() > _freeIndices.size())
  {
    size_t newParticlesCount = _particlesData.size() +
                                    _newParticles.size() - _freeIndices.size();
    _particlesData.reserve(newParticlesCount);
    _workIndices.reserve(newParticlesCount);
  }

  while (!_newParticles.empty())
  {
    ParticleData particle = _newParticles.back();
    _newParticles.pop_back();

    if (_freeIndices.empty())
    {
      ParticleIndex newIndex(_particlesData.size());
      _workIndices.push_back(newIndex);
      _particlesData.push_back(particle);
      newIndices.push_back(newIndex);
    }
    else
    {
      ParticleIndex newIndex = _freeIndices.back();
      _workIndices.push_back(newIndex);
      _freeIndices.pop_back();
      _particlesData[newIndex] = particle;
      newIndices.push_back(newIndex);
    }
  }

  if(!newIndices.empty()) emit particlesAdded(newIndices);
}

void ParticleField::_updateParticlesData(mtt::TimeT delta)
{
  float floatDelta = mtt::toFloatTime(delta);

  for (ParticleIndex index : _workIndices)
  {
    ParticleData& particle = _particlesData[index];
    particle.position += particle.speed * floatDelta;
    particle.rotation += particle.rotationSpeed * floatDelta;
    particle.currentTime += delta;
  }
}

void ParticleField::_deleteParticles()
{
  if(_workIndices.empty()) return;

  std::vector<ParticleIndex> deletedIndices;
  deletedIndices.reserve(_workIndices.size());

  glm::vec3 halfsize = _size / 2.f;

  std::vector<ParticleIndex>::iterator currentCursor = _workIndices.begin();
  std::vector<ParticleIndex>::iterator copyPlace = _workIndices.begin();

  while (currentCursor != _workIndices.end())
  {
    ParticleData& particle = _particlesData[*currentCursor];
    float particleRadius = particle.size / 2.f;
    if( particle.currentTime >= particle.maxTime ||
        !isfinite(particle.position.x) ||
        particle.position.x > halfsize.x - particleRadius ||
        particle.position.x < -halfsize.x + particleRadius ||
        !isfinite(particle.position.y) ||
        particle.position.y > halfsize.y - particleRadius ||
        particle.position.y < -halfsize.y + particleRadius ||
        !isfinite(particle.position.z) ||
        particle.position.z > halfsize.z - particleRadius ||
        particle.position.z < -halfsize.z + particleRadius)
    {
      deletedIndices.push_back(*currentCursor);
      _freeIndices.push_back(*currentCursor);
    }
    else
    {
      if(copyPlace != currentCursor) *copyPlace = *currentCursor;
      copyPlace++;
    }
    currentCursor++;
  }

  if (copyPlace != _workIndices.end())
  {
    _workIndices.erase(copyPlace, _workIndices.end());
  }

  emit particlesDeleted(deletedIndices);
}
