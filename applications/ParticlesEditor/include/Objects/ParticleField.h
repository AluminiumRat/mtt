#pragma once

#include <vector>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <mtt/application/TimeT.h>
#include <mtt/editorLib/Objects/MovableObject.h>
#include <mtt/particles/Drawable/ParticlesDrawable.h>

#include <Objects/ParticleTextureDescription.h>
#include <Objects/PEVisitorExtension.h>

class EmitterObject;
class FluidObject;
class ModificatorObject;

class ParticleField : public mtt::MovableObject
{
  Q_OBJECT
  DEFINE_EXTENSION_ACCEPT(PEVisitorExtension,
                          visitParticleField,
                          visitConstParticleField,
                          mtt::MovableObject)

public:
  using ParticleIndex = uint16_t;

  struct ParticleData
  {
    uint32_t typeMask;
    glm::vec3 position;
    glm::vec3 speed;
    float size;
    float rotation;
    float rotationSpeed;
    glm::vec4 albedo;
    glm::vec3 emission;
    float visibilityFactor;
    uint8_t textureIndex;
    uint8_t tileIndex;
    mtt::TimeT currentTime;
    mtt::TimeT maxTime;
    float mass;
    float frictionFactor;
  };

  enum ModificatorApplyTime
  {
    PREFLUID_TIME,
    POSTFLUID_TIME,
    POSTUPDATE_TIME
  };

  enum LightingType
  {
    PER_PARTICLE_LIGHTING = mtt::ParticlesDrawable::PER_PARTICLE_LIGHTING,
    PER_PIXEL_LIGHTING = mtt::ParticlesDrawable::PER_PIXEL_LIGHTING
  };
  const static std::map<LightingType, QString> lightingTypeNames;

  Q_PROPERTY( glm::vec3 size
              READ size
              WRITE setSize
              NOTIFY sizeChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( ParticleTextureDescriptions textureDescriptions
              READ textureDescriptions
              WRITE setTextureDescriptions
              NOTIFY textureDescriptionsChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( float lodMppx
              READ lodMppx
              WRITE setLodMppx
              NOTIFY lodMppxChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( float lodSmoothing
              READ lodSmoothing
              WRITE setLodSmoothing
              NOTIFY lodSmoothingChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( LightingType lightingType
              READ lightingType
              WRITE setLightingType
              NOTIFY lightingTypeChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

public:
  ParticleField(const QString& name,
                bool canBeRenamed,
                const mtt::UID& id = mtt::UID());
  ParticleField(const ParticleField&) = delete;
  ParticleField& operator = (const ParticleField&) = delete;
  virtual ~ParticleField() noexcept = default;

  inline const glm::vec3& size() const noexcept;
  void setSize(const glm::vec3& newValue);

  inline const std::vector<ParticleIndex>& workIndices() const noexcept;
  inline const std::vector<ParticleData>& particlesData() const noexcept;

  /// Does not add particle at once, but adds it to the queue. The particle will
  /// be added before next simulation step.
  void addParticle(const ParticleData& particle);
  /// Remove all particles
  void clear() noexcept;

  /// delegate must have signature void(ParticleData&)
  template<typename DelegateType>
  inline void updateParticles(DelegateType theDelegate, uint32_t typeMask);

  inline const ParticleTextureDescriptions&
                                          textureDescriptions() const noexcept;
  void setTextureDescriptions(
                            const ParticleTextureDescriptions& newDescriptions);

  inline float lodMppx() const noexcept;
  void setLodMppx(float newValue) noexcept;

  inline float lodSmoothing() const noexcept;
  void setLodSmoothing(float newValue) noexcept;

  inline LightingType lightingType() const noexcept;
  void setLightingType(LightingType newValue) noexcept;

  inline FluidObject& fluid() noexcept;
  inline const FluidObject& fluid() const noexcept;

  void registerEmitter(EmitterObject& emitter);
  void unregisterEmitter(EmitterObject& emitter) noexcept;

  void registerModificator(ModificatorObject& modificator);
  void unregisterModificator(ModificatorObject& modificator) noexcept;

  void simulationStep(mtt::TimeRange time);

signals:
  void sizeChanged(const glm::vec3& newValue);
  void textureDescriptionsChanged(const ParticleTextureDescriptions& newValue);
  void lodMppxChanged(float newValue);
  void lodSmoothingChanged(float newValue);
  void lightingTypeChanged(LightingType newValue);
  void cleared();
  void simulationStepStarted();
  /// First step of simulation
  void particlesAdded(const std::vector<ParticleIndex>& indices);
  /// Second step of simulation
  void particlesDataChanged();
  /// Third step of simulation
  /// Indices have already been removed from the working indices array, but
  /// particle data is still available
  void particlesDeleted(const std::vector<ParticleIndex>& indices);
  void simulationStepFinished();

private:
  void _addParticles();
  void _applyModificators(mtt::TimeRange time, ModificatorApplyTime applyTime);
  void _updateParticlesData(mtt::TimeT delta);
  void _deleteParticles();

private:
  glm::vec3 _size;

  std::vector<ParticleIndex> _workIndices;
  std::vector<ParticleIndex> _freeIndices;
  std::vector<ParticleData> _particlesData;
  std::vector<ParticleData> _newParticles;

  ParticleTextureDescriptions _textureDescriptions;

  float _lodMppx;
  float _lodSmoothing;

  LightingType _lightingType;

  using Emitters = std::vector<EmitterObject*>;
  Emitters _emitters;

  using Modificators = std::vector<ModificatorObject*>;
  Modificators _modificators;

  FluidObject* _fluid;
};

inline const glm::vec3& ParticleField::size() const noexcept
{
  return _size;
}

inline const ParticleTextureDescriptions&
                            ParticleField::textureDescriptions() const noexcept
{
  return _textureDescriptions;
}

inline float ParticleField::lodMppx() const noexcept
{
  return _lodMppx;
}

inline float ParticleField::lodSmoothing() const noexcept
{
  return _lodSmoothing;
}

inline ParticleField::LightingType ParticleField::lightingType() const noexcept
{
  return _lightingType;
}

inline FluidObject& ParticleField::fluid() noexcept
{
  return *_fluid;
}

inline const FluidObject& ParticleField::fluid() const noexcept
{
  return *_fluid;
}

inline const std::vector<ParticleField::ParticleIndex>&
                                      ParticleField::workIndices() const noexcept
{
  return _workIndices;
}

inline const std::vector<ParticleField::ParticleData>&
                                    ParticleField::particlesData() const noexcept
{
  return _particlesData;
}

template<typename DelegateType>
inline void ParticleField::updateParticles( DelegateType theDelegate,
                                            uint32_t typeMask)
{
  for (ParticleIndex index : _workIndices)
  {
    ParticleData& particle = _particlesData[index];
    if ((particle.typeMask & typeMask) != 0)
    {
      theDelegate(particle);
    }
  }
}
