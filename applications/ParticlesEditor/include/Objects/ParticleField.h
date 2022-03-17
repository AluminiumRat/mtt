#pragma once

#include <vector>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <mtt/application/TimeT.h>
#include <mtt/editorLib/Objects/MovableObject.h>

#include <Objects/ParticleTextureDescription.h>
#include <Objects/PEVisitorExtension.h>

class ModificatorObject;

class ParticleField : public mtt::MovableObject
{
  Q_OBJECT
  DEFINE_EXTENSION_ACCEPT(PEVisitorExtension,
                          visitParticleField,
                          visitConstParticleField,
                          mtt::MovableObject)

  Q_PROPERTY( glm::vec3 size
              READ size
              WRITE setSize
              RESET resetSize
              NOTIFY sizeChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

  Q_PROPERTY( ParticleTextureDescriptions textureDescriptions
              READ textureDescriptions
              WRITE setTextureDescriptions
              RESET resetTextureDescriptions
              NOTIFY textureDescriptionsChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

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
    glm::vec3 color;
    float brightness;
    float opacity;
    uint8_t textureIndex;
    uint8_t tileIndex;
    mtt::TimeT currentTime;
    mtt::TimeT maxTime;
    float mass;
    float frictionFactor;
  };

public:
  ParticleField(const QString& name,
                bool canBeRenamed,
                const mtt::UID& id = mtt::UID());
  ParticleField(const ParticleField&) = delete;
  ParticleField& operator = (const ParticleField&) = delete;
  virtual ~ParticleField() noexcept = default;

  inline const glm::vec3& size() const noexcept;
  void setSize(const glm::vec3& newValue);
  inline void resetSize();

  inline const std::vector<ParticleIndex>& workIndices() const noexcept;
  inline const std::vector<ParticleData>& particlesData() const noexcept;

  /// Does not add particle at once, but adds it to the queue. The particle will
  /// be added before next simulation step.
  void addParticle(const ParticleData& particle);
  /// Remove all particles
  void clear() noexcept;

  inline const ParticleTextureDescriptions&
                                          textureDescriptions() const noexcept;
  void setTextureDescriptions(
                            const ParticleTextureDescriptions& newDescriptions);
  inline void resetTextureDescriptions();

  void registerModificator(ModificatorObject& modificator);
  void unregisterModificator(ModificatorObject& modificator) noexcept;

  void simulationStep(mtt::TimeT currentTime, mtt::TimeT delta);

signals:
  void sizeChanged(const glm::vec3& newValue);
  void textureDescriptionsChanged(const ParticleTextureDescriptions& newValue);
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
  void _updateParticlesData(mtt::TimeT delta);
  void _deleteParticles();

private:
  glm::vec3 _size;

  std::vector<ParticleIndex> _workIndices;
  std::vector<ParticleIndex> _freeIndices;
  std::vector<ParticleData> _particlesData;
  std::vector<ParticleData> _newParticles;

  ParticleTextureDescriptions _textureDescriptions;

  using Modificators = std::vector<ModificatorObject*>;
  Modificators _modificators;
};

inline const glm::vec3& ParticleField::size() const noexcept
{
  return _size;
}

inline void ParticleField::resetSize()
{
  setSize(glm::vec3(10.f, 10.f, 10.f));
}

inline const ParticleTextureDescriptions&
                            ParticleField::textureDescriptions() const noexcept
{
  return _textureDescriptions;
}

inline void ParticleField::resetTextureDescriptions()
{
  setTextureDescriptions(ParticleTextureDescriptions());
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
