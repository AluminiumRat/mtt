#pragma once

#include <memory>
#include <optional>

#include <glm/vec4.hpp>

#include <mtt/clPipeline/MeshTechniques/TransparentProxyTechnique.h>
#include <mtt/render/Drawable/Drawable.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/Pipeline/Buffer.h>
#include <mtt/render/Pipeline/GraphicsPipeline.h>
#include <mtt/render/Pipeline/Sampler.h>
#include <mtt/render/Pipeline/Texture2D.h>
#include <mtt/render/Pipeline/VolatileUniform.h>
#include <mtt/render/SceneGraph/AreaModificator.h>

namespace mtt
{
  class AbstractRenderPass;
}

class ParticlesDrawable : public mtt::Drawable
{
public:
  struct TextureData
  {
    std::shared_ptr<mtt::Texture2D> texture;
    uint8_t extent;
  };

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
                std::vector<uint32_t> textureIndexData,
                std::vector<uint32_t> tileIndexData);

  void setParticleTextures(const std::vector<TextureData>& textures);

  virtual void registerAreaModificators(mtt::AreaModificatorSet& set) override;
  virtual void unregisterAreaModificators(
                                mtt::AreaModificatorSet& set) noexcept override;

protected:
  virtual void buildDrawActions(mtt::DrawPlanBuildInfo& buildInfo) override;

private:
  class ProxyTechnique : public mtt::ModificatorProxyTechnique
  {
  public:
    ProxyTechnique(ParticlesDrawable& parent);

    void resetPipelines() noexcept;

  protected:
    virtual std::unique_ptr<mtt::AbstractMeshTechnique>
                        createTechnique(mtt::AreaModificatorSet& set) override;

  private:
    ParticlesDrawable& _parent;
  };

  class DrawTechnique : public mtt::AbstractMeshTechnique,
                        private mtt::AreaModificator::Consumer
  {
  public:
    DrawTechnique(ParticlesDrawable& parent,
                  mtt::AreaModificatorSet& modificatorSet);
    virtual ~DrawTechnique() noexcept;

    virtual void resetPipeline() noexcept override;
    virtual void addToDrawPlan(mtt::DrawPlanBuildInfo& buildInfo) override;

  private:
    std::string _makeTextureExtentDefine() const;
    void _rebuildPipeline(mtt::AbstractRenderPass& renderPass);
    void _applyAreaModifictors( mtt::ShaderModule& shader,
                                mtt::GraphicsPipeline& pipeline);
    mtt::Ref<mtt::PlainBuffer> _makeIndices(
                                const mtt::DrawPlanBuildInfo& buildInfo,
                                mtt::LogicalDevice& device) const;

  private:
    ParticlesDrawable& _parent;
    mtt::AreaModificatorSet& _modificatorSet;
    using MatricesUniform = mtt::VolatileUniform<mtt::DrawMatrices>;
    MatricesUniform _matricesUniform;
    std::optional<mtt::GraphicsPipeline> _pipeline;
  };

private:
  mtt::Buffer _positionBuffer;
  mtt::Buffer _sizeRotationBuffer;
  mtt::Buffer _colorBuffer;
  mtt::Buffer _textureIndexBuffer;
  mtt::Buffer _tileIndexBuffer;
  size_t _particlesNumber;

  std::optional<mtt::Sampler> _sampler;

  std::vector<TextureData> _textureData;
  std::vector<glm::vec3> _positionsData;

  ProxyTechnique _technique;
};