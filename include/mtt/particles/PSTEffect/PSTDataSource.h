#pragma once

#include <QtCore/Qstring>

#include <string>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <mtt/application/TimeT.h>
#include <mtt/particles/Drawable/ParticlesDrawCommonData.h>
#include <mtt/render/Pipeline/Texture2D.h>
#include <mtt/utilities/Sphere.h>

class QDir;
class QFile;

namespace mtt
{
  class DataStream;
  class LogicalDevice;
  class Texture2DLibrary;

  class PSTDataSource
  {
  public:
    static const inline std::string fileHead = "PSTFile";
    static constexpr uint32_t fileVersion = 0;

    static constexpr uint32_t notIndex = std::numeric_limits<uint16_t>::max();

  public:
    /// If textureLibrary is nullptr then textures will be loaded without
    /// resource management
    PSTDataSource(const QString& filename,
                  Texture2DLibrary* textureLibrary,
                  LogicalDevice& device);
    PSTDataSource(const PSTDataSource&) = delete;
    PSTDataSource& operator = (const PSTDataSource&) = delete;
    virtual ~PSTDataSource() noexcept = default;

    inline TimeT duration() const noexcept;
    inline const Sphere& boundSphere() const noexcept;

    inline const ParticlesDrawCommonData::TextureDataSet&
                                                      textures() const noexcept;

    inline float lodMppx() const noexcept;
    inline float lodSmoothing() const noexcept;

    /// x component of sizeRotation is diameter of particle,
    /// y component of sizeRotation is rotation of particle in radians
    /// albedoData contains premultipled rgba color value with opacity
    /// emissionData uses only rgb components
    /// tagData is random number.
    void updateData(std::vector<glm::vec3>& positionData,
                    std::vector<glm::vec2>& sizeRotationData,
                    std::vector<glm::vec4>& albedoData,
                    std::vector<glm::vec4>& emissionData,
                    std::vector<uint32_t>& textureIndexData,
                    std::vector<uint32_t>& tileIndexData,
                    std::vector<uint8_t>& tagData,
                    TimeT time) const;

  private:
    struct ParticleData
    {
      glm::vec3 position;
      glm::vec2 sizerotation;
      glm::vec4 albedo;
      glm::vec4 emission;
      uint8_t textureIndex;
      uint8_t tileIndex;
      uint8_t tag;
      uint16_t nextIndex;
    };
    using Particles = std::vector<ParticleData>;

    struct Frame
    {
      TimeT time;
      Particles particles;

      Frame() = default;
      Frame(const Frame&) = default;
      Frame(Frame&&) = default;
      Frame& operator = (const Frame&) = default;
      Frame& operator = (Frame&&) = default;
      ~Frame() noexcept = default;
    };
    using Frames = std::vector<Frame>;

    struct FramesPosition
    {
      const Frame* prev;
      const Frame* next;
      float interpolationFactor;
    };

  private:
    void _checkHead(QFile& file, DataStream& stream);
    void _loadTextures( DataStream& stream,
                        const QDir& fileDirectory,
                        Texture2DLibrary* textureLibrary,
                        LogicalDevice& device);
    void _loadFrames(DataStream& stream);
    FramesPosition findFrames(TimeT time) const noexcept;

  private:
    Frames _frames;
    TimeT _duration;
    Sphere _boundSphere;

    ParticlesDrawCommonData::TextureDataSet _textures;
    float _lodMppx;
    float _lodSmoothing;
  };

  inline TimeT PSTDataSource::duration() const noexcept
  {
    return _duration;
  }

  inline const Sphere& PSTDataSource::boundSphere() const noexcept
  {
    return _boundSphere;
  }

  inline const ParticlesDrawCommonData::TextureDataSet&
                                        PSTDataSource::textures() const noexcept
  {
    return _textures;
  }

  inline float PSTDataSource::lodMppx() const noexcept
  {
    return _lodMppx;
  }

  inline float PSTDataSource::lodSmoothing() const noexcept
  {
    return _lodSmoothing;
  }
}