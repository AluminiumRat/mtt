#pragma once

#include <optional>
#include <vector>

#include <mtt/render/Pipeline/Buffer.h>
#include <mtt/render/Pipeline/Sampler.h>
#include <mtt/render/Pipeline/Texture2D.h>

class ParticlesDrawCommonData
{
public:
  struct TextureData
  {
    std::shared_ptr<mtt::Texture2D> texture;
    uint8_t extent;
  };

public:
  ParticlesDrawCommonData();
  ParticlesDrawCommonData(const ParticlesDrawCommonData&) = delete;
  ParticlesDrawCommonData& operator = (const ParticlesDrawCommonData&) = delete;
  ~ParticlesDrawCommonData() noexcept = default;

public:
  mtt::Buffer positionBuffer;
  mtt::Buffer sizeRotationBuffer;
  mtt::Buffer albedoBuffer;
  mtt::Buffer emissionBuffer;
  mtt::Buffer textureIndexBuffer;
  mtt::Buffer tileIndexBuffer;
  mtt::Buffer falloffFactorBuffer;
  size_t particlesNumber;

  std::optional<mtt::Sampler> textureSampler;
  mtt::Sampler depthSampler;
  mtt::Texture2D* depthSamplerTexture;

  std::vector<TextureData> textureData;
  std::vector<glm::vec3> positionsData;
  std::vector<float> falloffFactorData;
  std::vector<uint8_t> tagData;

  float falloffBaseMppx;
  float falloffSmoothing;
};