#pragma once

#include <memory>
#include <optional>

#include <glm/vec4.hpp>

#include <mtt/render/Drawable/Drawable.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/Pipeline/Buffer.h>
#include <mtt/render/Pipeline/GraphicsPipeline.h>
#include <mtt/render/Pipeline/Sampler.h>
#include <mtt/render/Pipeline/Texture2D.h>
#include <mtt/render/Pipeline/VolatileUniform.h>

namespace mtt
{
  class AbstractRenderPass;
}

class ParticlesDrawable : public mtt::Drawable
{
public:
  ParticlesDrawable();
  ParticlesDrawable(const ParticlesDrawable&) = delete;
  ParticlesDrawable& operator = (const ParticlesDrawable&) = delete;
  virtual ~ParticlesDrawable() = default;

  /// All data vectors must have the same size
  /// x component of sizeRotation is diameter of particle,
  /// y component of sizeRotation is rotation of particle in radians
  /// colorData contains premultipled rgba color value with opacity
  /// If particlesNumber != 0 then all pointers must not be nullptr
  void setData( std::vector<glm::vec3> positionData,
                std::vector<glm::vec2> sizeRotationData,
                std::vector<glm::vec4> colorData,
                std::vector<uint32_t> textureIndexData);

  void setParticleTextures(
                  const std::vector<std::shared_ptr<mtt::Texture2D>>& textures);

protected:
  virtual void buildDrawActions(mtt::DrawPlanBuildInfo& buildInfo) override;

private:
  class DrawTechnique
  {
  public:
    DrawTechnique(ParticlesDrawable& parent);

    void resetPipeline() noexcept;
    void buildDrawActions(mtt::DrawPlanBuildInfo& buildInfo);

  private:
    void _rebuildPipeline(mtt::AbstractRenderPass& renderPass);
    mtt::Ref<mtt::PlainBuffer> _makeIndices(
                                const mtt::DrawPlanBuildInfo& buildInfo,
                                mtt::LogicalDevice& device) const;

  private:
    ParticlesDrawable& _parent;
    using MatricesUniform = mtt::VolatileUniform<mtt::DrawMatrices>;
    MatricesUniform _matricesUniform;
    std::optional<mtt::GraphicsPipeline> _pipeline;
  };

private:
  mtt::Buffer _positionBuffer;
  mtt::Buffer _sizeRotationBuffer;
  mtt::Buffer _colorBuffer;
  mtt::Buffer _textureIndexBuffer;
  size_t _particlesNumber;

  std::optional<mtt::Sampler> _sampler;

  std::vector<glm::vec3> _positionsData;

  DrawTechnique _colorTechnique;
};