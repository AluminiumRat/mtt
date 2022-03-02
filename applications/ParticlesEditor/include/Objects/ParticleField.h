#pragma once

#include <vector>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <mtt/application/Application.h>
#include <mtt/editorLib/Objects/MovableObject.h>

#include <Objects/PEVisitorExtension.h>

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

  Q_PROPERTY( std::vector<QString> textureFiles
              READ textureFiles
              WRITE setTextureFiles
              RESET resetTextureFiles
              NOTIFY textureFilesChanged
              DESIGNABLE true
              SCRIPTABLE true
              STORED true
              USER false)

public:
  using TimeType = mtt::Application::TimeType;

  using ParticleIndex = uint16_t;

  struct ParticleData
  {
    uint8_t typeIndex;
    glm::vec3 position;
    glm::vec3 speed;
    float size;
    float rotation;
    float rotationSpeed;
    glm::vec3 color;
    float brightness;
    float opacity;
    uint8_t textureIndex;
    TimeType currentTime;
    TimeType maxTime;
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

  inline const std::vector<QString>& textureFiles() const noexcept;
  void setTextureFiles(const std::vector<QString>& newFiles);
  inline void resetTextureFiles();

  void simulationStep(TimeType delta);

signals:
  void sizeChanged(const glm::vec3& newValue);
  void textureFilesChanged(const std::vector<QString>& newValue);
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
  void _updateParticlesData(TimeType delta);
  void _deleteParticles();

private:
  glm::vec3 _size;

  std::vector<ParticleIndex> _workIndices;
  std::vector<ParticleIndex> _freeIndices;
  std::vector<ParticleData> _particlesData;
  std::vector<ParticleData> _newParticles;

  std::vector<QString> _textureFiles;
};

inline const glm::vec3& ParticleField::size() const noexcept
{
  return _size;
}

inline void ParticleField::resetSize()
{
  setSize(glm::vec3(10.f, 10.f, 10.f));
}

inline const std::vector<QString>& ParticleField::textureFiles() const noexcept
{
  return _textureFiles;
}

inline void ParticleField::resetTextureFiles()
{
  setTextureFiles(std::vector<QString>());
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
