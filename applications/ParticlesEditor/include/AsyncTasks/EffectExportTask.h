#pragma once

#include <limits>
#include <vector>

#include <QtCore/QObject>

#include <mtt/application/AsyncTasks/SaveToFileTask.h>
#include <mtt/application/EditCommands/AbstractEditCommand.h>
#include <mtt/application/Scene/Scene.h>
#include <mtt/application/TimeT.h>
#include <mtt/utilities/Sphere.h>

#include <Objects/ParticleField.h>
#include <Objects/RootObject.h>

class ParticlesEditorScene;

class EffectExportTask :  public QObject,
                          public mtt::SaveToFileTask
{
  Q_OBJECT

public:
  struct ExportOptions
  {
    mtt::TimeT startTime;   // Must be greater than or equal to 0
    mtt::TimeT duration;    // Must be greater than 0
    mtt::TimeT timeStep;    // Must be greater than 0
  };

public:
  EffectExportTask( ParticlesEditorScene& scene,
                    const QString& filename,
                    ExportOptions options);
  EffectExportTask(const EffectExportTask&) = delete;
  EffectExportTask& operator = (const EffectExportTask&) = delete;
  virtual ~EffectExportTask() noexcept = default;

protected:
  virtual void preparePart() override;
  virtual void saveData(QFile& file,
                        mtt::DataStream& stream,
                        const QFileInfo& targetFileInfo,
                        const QFileInfo& tmpFileInfo) override;
  virtual void finalizePart() override;
  virtual void restorePart() noexcept override;

private:
  struct Particle
  {
    glm::vec3 position;
    float size;
    float rotation;
    glm::vec4 albedo;
    glm::vec3 emission;
    uint8_t textureIndex;
    uint8_t tileIndex;
    mtt::TimeT currentTime;
    mtt::TimeT maxTime;
    uint8_t tag;
    uint16_t currentIndex;
    uint16_t nextIndex;
  };
  using Particles = std::vector<Particle>;

  struct Frame
  {
    mtt::TimeT time;
    qint64 position;
    Particles particles;

    Frame() = default;
    Frame(const Frame&) = default;
    Frame(Frame&&) = default;
    Frame& operator = (const Frame&) = default;
    Frame& operator = (Frame&&) = default;
    ~Frame() noexcept = default;
  };
  using Frames = std::vector<Frame>;

  void _writeHead(QFile& file, mtt::DataStream& stream);
  void _writeFieldInfo(mtt::DataStream& stream, const QFileInfo& targetFileInfo);
  void _addFrame(mtt::TimeT time);
  void _markDeleted(const std::vector<ParticleField::ParticleIndex>& indices);
  void _connectFrames();
  void _writeFrames(QFile& file, mtt::DataStream& stream);
  void _writeIndex(QFile& file, mtt::DataStream& stream);
  void _restoreScene() noexcept;

private:
  ExportOptions _options;
  ParticlesEditorScene& _originalScene;
  mtt::Scene _workScene;
  std::unique_ptr<RootObject> _sceneData;
  std::unique_ptr<mtt::AbstractEditCommand> _restoreCommand;

  Frames _frames;
  mtt::Sphere _boundSphere;

  qint64 _indexPtrPos;

  uint8_t _randomTable[256];
};