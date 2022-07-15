#pragma once

#include <optional>
#include <vector>

#include <mtt/render/Pipeline/Buffer.h>
#include <mtt/render/Pipeline/Sampler.h>
#include <mtt/render/Pipeline/Texture2D.h>

namespace mtt
{
  class ParticlesDrawCommonData
  {
  public:
    enum LightingType
    {
      PER_PARTICLE_LIGHTING,
      PER_PIXEL_LIGHTING
    };

    struct TextureData
    {
      std::shared_ptr<Texture2D> texture;
      uint8_t extent;                       /// Extent of texture in tiles in
                                            /// one dimension
    };
    using TextureDataSet = std::vector<TextureData>;

  public:
    explicit ParticlesDrawCommonData(LogicalDevice& theDevice);
    ParticlesDrawCommonData(const ParticlesDrawCommonData&) = delete;
    ParticlesDrawCommonData& operator = (
                                      const ParticlesDrawCommonData&) = delete;
    ~ParticlesDrawCommonData() noexcept = default;

  public:
    LogicalDevice& device;

    Buffer positionBuffer;
    Buffer sizeRotationBuffer;
    Buffer albedoBuffer;
    Buffer emissionBuffer;
    Buffer textureIndexBuffer;
    Buffer tileIndexBuffer;
    Buffer falloffFactorBuffer;
    size_t particlesNumber;

    std::optional<Sampler> textureSampler;
    Sampler depthSampler;
    Texture2D* depthSamplerTexture;

    TextureDataSet textureData;
    std::vector<glm::vec3> positionsData;
    std::vector<float> falloffFactorData;
    std::vector<uint8_t> tagData;

    float falloffBaseMppx;
    float falloffSmoothing;

    LightingType lightingType;
  };
}