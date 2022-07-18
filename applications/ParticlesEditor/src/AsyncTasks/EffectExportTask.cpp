#include <memory>
#include <random>

#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QFile>

#include <mtt/particles/PSTEffect/PSTDataSource.h>
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
  file.write( mtt::PSTDataSource::fileHead.c_str(),
              mtt::PSTDataSource::fileHead.length());
  stream << mtt::PSTDataSource::fileVersion;
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
  try
  {
    _sceneData->particleField().clear();

    mtt::TimeT timeCursor(0);
    reportStage(QObject::tr("presimulation"));
    _presimulation(timeCursor);

    connect(&_sceneData->particleField(),
            &ParticleField::particlesDeleted,
            this,
            &EffectExportTask::_markDeleted,
            Qt::DirectConnection);

    reportStage(QObject::tr("simulation"));
    _simulation(timeCursor);

    _finalFramesNumber = _frames.size();

    if (_options.looped)
    {
      reportStage(QObject::tr("postsimulation"));
      _postsimulation(timeCursor);
    }

    disconnect( &_sceneData->particleField(),
                &ParticleField::particlesDeleted,
                this,
                &EffectExportTask::_markDeleted);

    reportStage(QObject::tr("frames connection"));
    _connectFrames();

    if (_options.looped)
    {
      reportStage(QObject::tr("overlap"));
      _overlapFrames();
    }

    reportStage(QObject::tr("header writing"));
    _writeHead(file, stream);

    stream << _boundSphere.center;
    stream << _boundSphere.radius;
    _writeFieldInfo(stream, targetFileInfo);
    stream << _options.looped;

    _indexPtrPos = file.pos();
    stream << qint64(0);          // Write empty index pointer

    reportStage(QObject::tr("frames writing"));
    _writeFrames(file, stream);

    reportStage(QObject::tr("index writing"));
    _writeIndex(file, stream);
  }
  catch (...)
  {
    disconnect(this);
    throw;
  }
}

void EffectExportTask::_presimulation(mtt::TimeT& timeCursor)
{
  for (; timeCursor < _options.startTime; timeCursor += _options.timeStep)
  {
    mtt::TimeRange frameTime(timeCursor, timeCursor + _options.timeStep);
    _sceneData->animation().update(frameTime);
    reportPercent(timeCursor.count() * 100 / _options.startTime.count());
  }

  if (_options.looped)
  {
    _addToCanceled(_sceneData->particleField().workIndices());
  }
}

void EffectExportTask::_simulation(mtt::TimeT& timeCursor)
{
  for ( ;
        timeCursor < _options.startTime + _options.duration;
        timeCursor += _options.timeStep)
  {
    mtt::TimeRange frameTime(timeCursor, timeCursor + _options.timeStep);
    _sceneData->animation().update(frameTime);

    _addFrame(frameTime.finish() - _options.startTime);

    reportPercent((timeCursor - _options.startTime).count() * 100 /
                                                    _options.duration.count());
  }
}

void EffectExportTask::_postsimulation(mtt::TimeT& timeCursor)
{
  if(_frames.empty()) return;

  connect(&_sceneData->particleField(),
          &ParticleField::particlesAdded,
          this,
          &EffectExportTask::_addToCanceled,
          Qt::DirectConnection);

  while (!_frames.back().particles.empty())
  {
    mtt::TimeRange frameTime(timeCursor, timeCursor + _options.timeStep);
    _sceneData->animation().update(frameTime);
    _addFrame(frameTime.finish() - _options.startTime);

    timeCursor += _options.timeStep;
    if (_sceneData->animation().duration() < timeCursor) break;
  }

  _frames.pop_back();

  disconnect( &_sceneData->particleField(),
              &ParticleField::particlesAdded,
              this,
              &EffectExportTask::_addToCanceled);
}

void EffectExportTask::_addFrame(mtt::TimeT time)
{
  ParticleField& field = _sceneData->particleField();

  Frame newFrame;
  newFrame.time = time;
  newFrame.particles.reserve(field.workIndices().size());

  for (ParticleField::ParticleIndex index : field.workIndices())
  {
    if(_canceled.contains(index)) continue;

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

    _boundSphere.extend(mtt::Sphere(data.position, data.size / 2.f));
  }

  _frames.push_back(std::move(newFrame));
}

void EffectExportTask::_markDeleted(
                      const std::vector<ParticleField::ParticleIndex>& indices)
{
  for (ParticleField::ParticleIndex deletedIndex : indices)
  {
    _canceled.erase(deletedIndex);
  }

  if(_frames.empty()) return;
  Frame& lastFrame = _frames.back();

  for (ParticleField::ParticleIndex deletedIndex : indices)
  {
    for (Particle& particle : lastFrame.particles)
    {
      if (particle.nextIndex == deletedIndex)
      {
        particle.nextIndex = mtt::PSTDataSource::notIndex;
        particle.albedo.a = 0;
        particle.emission = glm::vec3(0);
        break;
      }
    }
  }
}

void EffectExportTask::_addToCanceled(
                      const std::vector<ParticleField::ParticleIndex>& indices)
{
  for (ParticleField::ParticleIndex index : indices)
  {
    _canceled.insert(index);
  }
}

void EffectExportTask::_connectFrames()
{
  for (size_t frameIndex = 0; frameIndex < _frames.size() - 1; frameIndex++)
  {
    Frame& currentFrame = _frames[frameIndex];
    Frame& nextFrame = _frames[frameIndex + 1];

    uint16_t nextParticleIndex = 0;

    for (Particle& currentFrameParticle : currentFrame.particles)
    {
      if(currentFrameParticle.nextIndex == mtt::PSTDataSource::notIndex)
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

  for (Particle& particle : _frames.back().particles)
  {
    particle.nextIndex = mtt::PSTDataSource::notIndex;
    particle.albedo.a = 0;
    particle.emission = glm::vec3(0);
  }
}

void EffectExportTask::_overlapFrames()
{
  if(_finalFramesNumber <= 1) return;

  for (Particle& particle : _frames[_finalFramesNumber - 1].particles)
  {
    particle.nextIndex += _frames[0].particles.size();
  }

  for(size_t currentFrameIndex = _finalFramesNumber;
      currentFrameIndex < _frames.size();
      currentFrameIndex++)
  {
    size_t targetFrameIndex = currentFrameIndex % _finalFramesNumber;
    Particles& targetPlace = _frames[targetFrameIndex].particles;

    size_t nextFrameIndex = (targetFrameIndex + 1) % _finalFramesNumber;
    uint16_t indexIncrement =
                            uint16_t(_frames[nextFrameIndex].particles.size());

    for (Particle& particle : _frames[currentFrameIndex].particles)
    {
      if (particle.nextIndex != mtt::PSTDataSource::notIndex)
      {
        particle.nextIndex += indexIncrement;
      }
      targetPlace.push_back(particle);
    }
  }

  _frames.resize(_finalFramesNumber);
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
