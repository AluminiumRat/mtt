#include <memory>
#include <random>

#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QFile>

#include <mtt/application/ParticlesDataSource/ParticlesDataSource.h>
#include <mtt/utilities/Abort.h>

#include <AsyncTasks/EffectExportTask.h>
#include <ParticlesEditorScene.h>

EffectExportTask::EffectExportTask( ParticlesEditorScene& scene,
                                    const QString& filename,
                                    ExportOptions options) :
  SaveToFileTask(QObject::tr("Saving"), filename),
  _options(options),
  _originalScene(scene)
{
  if(options.startTime.count() < 0) mtt::Abort("EffectExportTask::EffectExportTask: start time is negative");
  if(options.timeStep.count() <= 0) mtt::Abort("EffectExportTask::EffectExportTask: time step is null or negative");
  if(options.duration.count() <= 0) mtt::Abort("EffectExportTask::EffectExportTask: duration is null or negative");

  std::default_random_engine randomEngine;
  std::uniform_int_distribution<int> distribution(0, 255);
  for (uint8_t& value : _randomTable)
  {
    value = uint8_t(distribution(randomEngine));
  }
}

void EffectExportTask::preparePart()
{
  _restoreCommand = _originalScene.dataRoot().animation().makeRestoreCommand();
  _sceneData = _originalScene.changeDataRoot(
                                      std::make_unique<RootObject>("", false));
  _originalScene.unregisterObject(*_sceneData);
  _workScene.registerObject(*_sceneData);
}

void EffectExportTask::_writeHead(QFile& file, mtt::DataStream& stream)
{
  file.write( mtt::ParticlesDataSource::fileHead.c_str(),
              mtt::ParticlesDataSource::fileHead.length());
  stream << mtt::ParticlesDataSource::fileVersion;
}

void EffectExportTask::_writeFieldInfo( mtt::DataStream& stream,
                                        const QFileInfo& targetFileInfo)
{
  ParticleField& field = _sceneData->particleField();
  uint8_t texturesNumber = uint8_t(field.textureDescriptions().size());
  stream << texturesNumber;

  for ( uint8_t textureIndex = 0;
        textureIndex < texturesNumber;
        textureIndex++)
  {
    const ParticleTextureDescription& description =
                                      field.textureDescriptions()[textureIndex];
    stream << targetFileInfo.dir().relativeFilePath(description.filename);
    stream << uint8_t(description.extent);
  }

  stream << field.lodMppx();
  stream << field.lodSmoothing();
}

void EffectExportTask::saveData(QFile& file,
                                mtt::DataStream& stream,
                                const QFileInfo& targetFileInfo,
                                const QFileInfo& tmpFileInfo)
{
  reportStage(QObject::tr("header writing"));
  _writeHead(file, stream);
  _writeFieldInfo(stream, targetFileInfo);

  _indexPtrPos = file.pos();
  stream << qint64(0);

  _sceneData->particleField().clear();

  connect(&_sceneData->particleField(),
          &ParticleField::particlesDeleted,
          this,
          &EffectExportTask::_markDeleted,
          Qt::DirectConnection);

  reportStage(QObject::tr("simulation"));
  for ( mtt::TimeT frameStart(0);
        frameStart < _options.startTime + _options.duration;
        frameStart += _options.timeStep)
  {
    mtt::TimeRange frameTime(frameStart, frameStart + _options.timeStep);
    _sceneData->animation().update(frameTime);
    _addFrame(frameTime.finish());

    reportPercent(frameStart.count() * 100 /
                              (_options.startTime + _options.duration).count());
  }

  reportStage(QObject::tr("frames connection"));
  _connectFrames();

  disconnect( &_sceneData->particleField(),
              &ParticleField::particlesDeleted,
              this,
              &EffectExportTask::_markDeleted);

  reportStage(QObject::tr("frames writing"));
  _writeFrames(file, stream);

  reportStage(QObject::tr("index writing"));
  _writeIndex(file, stream);
}

void EffectExportTask::_addFrame(mtt::TimeT time)
{
  ParticleField& field = _sceneData->particleField();

  Frame newFrame;
  newFrame.time = time;
  newFrame.particles.reserve(field.workIndices().size());

  for (ParticleField::ParticleIndex index : field.workIndices())
  {
    const ParticleField::ParticleData& data = field.particlesData()[index];
    Particle newParticle;
    newParticle.position = data.position;
    newParticle.size = data.size;
    newParticle.rotation = data.rotation;
    newParticle.albedo = data.albedo;
    newParticle.albedo.a *= data.visibilityFactor;
    newParticle.emission = data.emission;
    newParticle.emission *= data.visibilityFactor;
    newParticle.textureIndex = data.textureIndex;
    newParticle.tileIndex = data.tileIndex;
    newParticle.currentTime = data.currentTime;
    newParticle.maxTime = data.maxTime;
    newParticle.tag = _randomTable[index % 256];
    newParticle.currentIndex = index;
    newParticle.nextIndex = index;

    newFrame.particles.push_back(newParticle);
  }

  _frames.push_back(std::move(newFrame));
}

void EffectExportTask::_markDeleted(
                      const std::vector<ParticleField::ParticleIndex>& indices)
{
  if(_frames.empty()) return;
  Frame& lastFrame = _frames.back();
  Particles::iterator iParticle = lastFrame.particles.begin();

  for (ParticleField::ParticleIndex deletedIndex : indices)
  {
    for (Particle& particle : lastFrame.particles)
    {
      if (particle.nextIndex == deletedIndex)
      {
        particle.nextIndex = mtt::ParticlesDataSource::notIndex;
        break;
      }
    }
  }
}

void EffectExportTask::_connectFrames()
{
  for (size_t frameIndex = 0; frameIndex < _frames.size() - 1; frameIndex++)
  {
    Frame& currentFrame = _frames[frameIndex];
    Frame& nextFrame = _frames[frameIndex++];

    uint16_t nextParticleIndex = 0;

    for (Particle& currentFrameParticle : currentFrame.particles)
    {
      if(currentFrameParticle.nextIndex == mtt::ParticlesDataSource::notIndex)
      {
        continue;
      }

      for (;nextParticleIndex < nextFrame.particles.size(); nextParticleIndex++)
      {
        Particle& nextFrameParticle = nextFrame.particles[nextParticleIndex];
        if (currentFrameParticle.nextIndex == nextFrameParticle.currentIndex)
        {
          currentFrameParticle.nextIndex = nextParticleIndex;
          break;
        }
      }
      if(nextParticleIndex == nextFrame.particles.size()) mtt::Abort("EffectExportTask::_connectFrames: next frame index not found");
    }
  }
}

void EffectExportTask::_writeFrames(QFile& file, mtt::DataStream& stream)
{
  stream << uint32_t(_frames.size());

  for (size_t frameIndex = 0; frameIndex < _frames.size(); frameIndex++)
  {
    Frame& frame = _frames[frameIndex];
    frame.position = file.pos();

    stream << frame.time;
    stream << uint16_t(frame.particles.size());

    for (const Particle& particle : frame.particles)
    {
      stream << particle.position;
      stream << particle.size;
      stream << particle.rotation;
      stream << particle.albedo;
      stream << particle.emission;
      stream << particle.textureIndex;
      stream << particle.tileIndex;
      stream << particle.tag;
      stream << particle.nextIndex;
    }

    reportPercent(frameIndex * 100 / _frames.size());
  }
}

void EffectExportTask::_writeIndex(QFile& file, mtt::DataStream& stream)
{
  qint64 indexPos = file.pos();

  stream << _frames.size();

  for (Frame& frame : _frames)
  {
    stream << frame.time;
    stream << frame.position;
  }

  file.seek(_indexPtrPos);
  stream << indexPos;

  file.seek(-1);
}

void EffectExportTask::finalizePart()
{
  _restoreScene();
}

void EffectExportTask::restorePart() noexcept
{
  _restoreScene();
}

void EffectExportTask::_restoreScene() noexcept
{
  try
  {
    if (_sceneData != nullptr)
    {
      _sceneData->particleField().clear();
      _workScene.unregisterObject(*_sceneData);
      _originalScene.changeDataRoot(std::move(_sceneData));
    }
    if (_restoreCommand != nullptr) _restoreCommand->make();
    _restoreCommand.reset();
  }
  catch (std::runtime_error& error)
  {
    mtt::Log() << error.what();
    mtt::Abort("EffectExportTask::restorePart: unable to restore initail state.");
  }
  catch (...)
  {
    mtt::Abort("EffectExportTask::restorePart: unable to restore initail state.");
  }
}
