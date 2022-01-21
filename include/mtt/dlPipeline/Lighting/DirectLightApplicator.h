#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <glm/vec4.hpp>

#include <mtt/dlPipeline/Lighting/AbstractLightApplicator.h>
#include <mtt/dlPipeline/Lighting/ShadowMapProvider.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/Pipeline/GraphicsPipeline.h>
#include <mtt/render/Pipeline/InputAttachment.h>
#include <mtt/render/Pipeline/Sampler.h>
#include <mtt/render/Pipeline/Texture2D.h>
#include <mtt/render/Pipeline/VolatileUniform.h>

namespace mtt
{
  class LogicalDevice;

  namespace dlPipeline
  {
    class DirectLightApplicator : public AbstractLightApplicator
    {
    public:
      explicit DirectLightApplicator(LogicalDevice& device);
      DirectLightApplicator(const DirectLightApplicator&) = delete;
      DirectLightApplicator& operator = (const DirectLightApplicator&) = delete;
      virtual ~DirectLightApplicator() = default;

      inline float radius() const noexcept;
      inline virtual void setRadius(float newValue) noexcept;

      inline ShadowMapProvider* shadowMapProvider() const noexcept;
      void setShadowMapProvider(ShadowMapProvider* newProvider) noexcept;

      inline size_t cascadeSize() const noexcept;
      /// newValue should not be 0
      void setCascadeSize(size_t newValue) noexcept;

      inline float blurSize() const noexcept;
      inline void setBlurSize(float newValue) noexcept;

    protected:
      virtual void buildDrawActions(DrawPlanBuildInfo& buildInfo) override;

    private:
      struct LightData
      {
        alignas(16) glm::vec3 illuminance;
        alignas(16) glm::vec3 lightInverseDirection;
        alignas(4) float distance;
        alignas(4) float radius;
        alignas(16) glm::mat4 clipToView;
        alignas(16) glm::mat4 viewToLocal;
      };

      struct ShadowMapInfo
      {
        ImageView* map;
        glm::vec4 coordCorrection;
      };
      using CascadeInfo = std::vector<ShadowMapInfo>;

      struct ShadowCorrectionData
      {
        alignas(4) float shadowFactorNullShift;
        alignas(4) float shadowFactorMultiplicator;
      };

      class DrawTechnique
      {
      public:
        DrawTechnique(bool transparentRender,
                      bool fullscreenRender,
                      DirectLightApplicator& parent);
        DrawTechnique(const DrawTechnique&) = delete;
        DrawTechnique& operator = (const DrawTechnique&) = delete;
        virtual ~DrawTechnique() = default;
  
        void invalidatePipeline() noexcept;
        void addToDrawPlan( DrawPlanBuildInfo& buildInfo,
                            const CascadeInfo& cascadeInfo);

      private:
        void _rebuildPipeline(AbstractRenderPass& renderPass, StageIndex stage);
        void _adjustPipeline();

        void _makeNonshadowCommand( DrawPlanBuildInfo& buildInfo,
                                    StageIndex stage,
                                    uint32_t pointsNumber,
                                    const LightData& lightData);

        void _makeShadowCommand(DrawPlanBuildInfo& buildInfo,
                                StageIndex stage,
                                uint32_t pointsNumber,
                                const LightData& lightData,
                                const CascadeInfo& cascadeInfo);

      private:
        bool _transparentRender;
        bool _fullscreenRender;
      
        std::optional<GraphicsPipeline> _pipeline;

        DirectLightApplicator& _parent;
      };

    private:
      void _rebuildShadowmapSampler();
      bool _fullscreen(const DrawPlanBuildInfo& buildInfo) const noexcept;
      CascadeInfo _createShadowMap(DrawPlanBuildInfo& buildInfo);
      ShadowMapProvider::Area _getTopArea(
                                  DrawPlanBuildInfo& buildInfo) const noexcept;
      glm::vec2 _getCascadeDirectionPoint(DrawPlanBuildInfo& buildInfo,
                                          glm::vec2 startPoint) const noexcept;
      ShadowMapProvider::Area _alignArea(
                          const ShadowMapProvider::Area& source) const noexcept;

    private:
      LogicalDevice& _device;

      ShadowMapProvider* _shadowMapProvider;

      Sampler _depthMapSampler;
      Texture2D* _depthTexture;

      Sampler _normalMapSampler;
      Texture2D* _normalTexture;

      Sampler _albedoMapSampler;
      Texture2D* _albedoTexture;

      Sampler _specularMapSampler;
      Texture2D* _specularTexture;

      std::optional<Sampler> _shadowmapSampler;
      std::vector<std::shared_ptr<Texture2D>> _shadowmapTextures;

      float _radius;
      VolatileUniform<LightData> _lightDataUniform;

      using CoordsCorrectionData = std::vector<glm::vec4>;
      VolatileUniform<CoordsCorrectionData> _coordsCorrectionUniform;
      VolatileUniform<DrawMatrices> _matricesUniform;

      size_t _cascadeSize;
      float _blurSize;

      struct TechniquesCollection
      {
        struct Set
        {
          std::unique_ptr<DrawTechnique> opaqueTechnique;
          std::unique_ptr<DrawTechnique> transparentTechnique;
        };

        Set shapeSet;
        Set fullscreenSet;
      };
      TechniquesCollection _techniques;
    };

    inline float DirectLightApplicator::radius() const noexcept
    {
      return _radius;
    }

    inline void DirectLightApplicator::setRadius(float newValue) noexcept
    {
      _radius = newValue;
    }

    inline ShadowMapProvider*
                      DirectLightApplicator::shadowMapProvider() const noexcept
    {
      return _shadowMapProvider;
    }

    inline size_t DirectLightApplicator::cascadeSize() const noexcept
    {
      return _cascadeSize;
    }

    inline float DirectLightApplicator::blurSize() const noexcept
    {
      return _blurSize;
    }

    inline void DirectLightApplicator::setBlurSize(float newValue) noexcept
    {
      _blurSize = newValue;
    }
  }
}