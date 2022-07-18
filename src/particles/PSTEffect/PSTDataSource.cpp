#include <memory>
#include <stdexcept>

#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QFile>

#include <mtt/application/ResourceManager/Texture2DLibrary.h>
#include <mtt/application/DataStream.h>
#include <mtt/particles/PSTEffect/PSTDataSource.h>

using namespace mtt;

PSTDataSource::PSTDataSource( const QString& filename,
                              Texture2DLibrary* textureLibrary,
                              LogicalDevice& device) :
  _duration(0),
  _looped(false),
  _lodMppx(0.f),
  _lodSmoothing(1.f)
{
  QFile file(filename);
  if (!file.open(QFile::ReadOnly)) throw std::runtime_error("Unable to open file");
  mtt::DataStream stream(&file);
  QDir fileDirectory(QFileInfo(filename).dir());

  _checkHead(file, stream);

  stream >> _boundSphere.center;
  stream >>_boundSphere.radius;

  _loadTextures(stream, fileDirectory, textureLibrary, device);

  stream >> _lodMppx;
  stream >> _lodSmoothing;

  stream >> _looped;

  stream.readInt64();

  _loadFrames(stream);

  if(!_frames.empty()) _duration = _frames.back().time;
}

void PSTDataSource::_checkHead(QFile& file, DataStream& stream)
{
  std::string head;
  head.resize(fileHead.size());
  file.read(head.data(), head.size());
  if(head != fileHead) throw std::runtime_error("Invalid file header");
  uint32_t fileVersion;
  stream >> fileVersion;
  if(fileVersion > fileVersion) throw std::runtime_error("Unsupported file version");
}

void PSTDataSource::_loadTextures(DataStream& stream,
                                        const QDir& fileDirectory,
                                        Texture2DLibrary* textureLibrary,
                                        LogicalDevice& device)
{
  uint8_t texturesNumber = stream.readUint8();
  if(texturesNumber == 0) return;

  std::unique_ptr<Texture2DLibrary> tmpLib;
  if (textureLibrary == nullptr)
  {
    tmpLib.reset(new Texture2DLibrary);
    textureLibrary = tmpLib.get();
  }

  for (uint8_t textureIndex = 0; textureIndex < texturesNumber; textureIndex++)
  {
    QString relativePath;
    stream >> relativePath;
    QFileInfo fileInfo(fileDirectory.absoluteFilePath(relativePath));
    QString textureFilename = fileInfo.canonicalFilePath();

    ParticlesDrawCommonData::TextureData newTexture;
    newTexture.texture = textureLibrary->load(textureFilename, device, true);
    stream >> newTexture.extent;

    _textures.push_back(newTexture);
  }
}

void PSTDataSource::_loadFrames(DataStream& stream)
{
  uint32_t framesNumber = stream.readUint32();
  _frames.reserve(framesNumber);

  for (uint32_t frameIndex = 0; frameIndex < framesNumber; frameIndex++)
  {
    Frame newFrame;
    stream >> newFrame.time;

    uint16_t particlesNumber = stream.readUint16();
    newFrame.particles.reserve(particlesNumber);

    for ( uint16_t particleIndex = 0;
          particleIndex < particlesNumber;
          particleIndex++)
    {
      ParticleData newParticle;
      stream >> newParticle.position;
      stream >> newParticle.sizerotation.x;
      stream >> newParticle.sizerotation.y;
      stream >> newParticle.albedo;
      newParticle.albedo.r *= newParticle.albedo.a;
      newParticle.albedo.g *= newParticle.albedo.a;
      newParticle.albedo.b *= newParticle.albedo.a;
      newParticle.emission = glm::vec4(stream.readVec3(), 1.f);
      stream >> newParticle.textureIndex;
      stream >> newParticle.tileIndex;
      stream >> newParticle.tag;
      stream >> newParticle.nextIndex;

      newFrame.particles.push_back(newParticle);
    }

    _frames.push_back(std::move(newFrame));
  }
}

void PSTDataSource::updateData( std::vector<glm::vec3>& positionData,
                                      std::vector<glm::vec2>& sizeRotationData,
                                      std::vector<glm::vec4>& albedoData,
                                      std::vector<glm::vec4>& emissionData,
                                      std::vector<uint32_t>& textureIndexData,
                                      std::vector<uint32_t>& tileIndexData,
                                      std::vector<uint8_t>& tagData,
                                      TimeT time) const
{
  positionData.clear();
  sizeRotationData.clear();
  albedoData.clear();
  textureIndexData.clear();
  tileIndexData.clear();
  tagData.clear();

  if(time.count() < 0) return;

  if(_looped) time = time % _duration;

  PSTDataSource::FramesPosition frames = findFrames(time);
  if (frames.prev == nullptr || frames.next == nullptr) return;

  size_t toReserve = frames.prev->particles.size();
  positionData.reserve(toReserve);
  sizeRotationData.reserve(toReserve);
  albedoData.reserve(toReserve);
  textureIndexData.reserve(toReserve);
  tileIndexData.reserve(toReserve);
  tagData.reserve(toReserve);

  float factor = frames.interpolationFactor;

  for (const ParticleData& prewParticle : frames.prev->particles)
  {
    if(prewParticle.nextIndex == notIndex) continue;
    const ParticleData& nextParticle =
                                frames.next->particles[prewParticle.nextIndex];

    positionData.push_back(glm::mix(prewParticle.position,
                                    nextParticle.position,
                                    factor));
    sizeRotationData.push_back(glm::mix(prewParticle.sizerotation,
                                        nextParticle.sizerotation,
                                        factor));
    albedoData.push_back(glm::mix(prewParticle.albedo,
                                  nextParticle.albedo,
                                  factor));
    emissionData.push_back(glm::mix(prewParticle.emission,
                                    nextParticle.emission,
                                    factor));
    textureIndexData.push_back(prewParticle.textureIndex);
    tileIndexData.push_back(prewParticle.tileIndex);
    tagData.push_back(prewParticle.tag);
  }
}

PSTDataSource::FramesPosition
                            PSTDataSource::findFrames(TimeT time) const noexcept
{
  if (time >= _duration || _frames.size() < 2)
  {
    return {nullptr, nullptr, 0.f};
  }

  if (time < _frames.front().time)
  {
    if (!_looped) return { nullptr, nullptr, 0.f };
    return {&_frames.back(),
            &_frames.front(),
            toFloatTime(time) / toFloatTime(_frames.front().time)};
  }

  size_t firstIndex = 0;
  size_t secondIndex = _frames.size() - 1;

  while (secondIndex - firstIndex > 1)
  {
    size_t middle = (firstIndex + secondIndex) / 2;
    if (_frames[middle].time > time) secondIndex = middle;
    else firstIndex = middle;
  }

  const Frame& firstFrame = _frames[firstIndex];
  const Frame& secondFrame = _frames[secondIndex];
  float factor = toFloatTime(time - firstFrame.time) /
                                toFloatTime(secondFrame.time - firstFrame.time);
  return {&firstFrame, &secondFrame, factor};
}
