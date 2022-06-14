#pragma once

#include <optional>

#include <mtt/clPipeline/Lighting/ShadowMapProvider.h>
#include <mtt/clPipeline/Lighting/SpotLightData.h>
#include <mtt/render/Pipeline/GraphicsPipeline.h>
#include <mtt/render/Pipeline/Sampler.h>
#include <mtt/render/Pipeline/Texture2D.h>
#include <mtt/render/Pipeline/VolatileUniform.h>
#include <mtt/render/SceneGraph/DrawableNode.h>

namespace mtt
{
  class LogicalDevice;

  namespace clPipeline
  {
    class SpotLight;

    class SpotLightApplicator : public DrawableNode
    {
    private:
      friend class SpotLight;
      SpotLightApplicator(SpotLight& light, LogicalDevice& device);
    public:
      SpotLightApplicator(const SpotLightApplicator&) = delete;
      SpotLightApplicator& operator = (const SpotLightApplicator&) = delete;
      virtual ~SpotLightApplicator() = default;

      void resetPipelines() noexcept;
      void updateBound() noexcept;

    protected:
      virtual void buildDrawActions(DrawPlanBuildInfo& buildInfo) override;

    private:
      class DrawTechnique
      {
      public:
        DrawTechnique(bool fullscreenRender,
                      SpotLight& light,
                      SpotLightApplicator& applicator);
        DrawTechnique(const DrawTechnique&) = delete;
        DrawTechnique& operator = (const DrawTechnique&) = delete;
        virtual ~DrawTechnique() = default;

        void invalidatePipeline() noexcept;
        void addToDrawPlan( DrawPlanBuildInfo& buildInfo,
                            ImageView* shadowMapView);

      private:
        void _rebuildPipeline(AbstractRenderPass& renderPass);
        void _adjustPipeline();

        void _makeNonshadowCommand( DrawPlanBuildInfo& buildInfo,
                                    uint32_t pointsNumber,
                                    const SpotLightData& lightData);

        void _makeShadowCommand(DrawPlanBuildInfo& buildInfo,
                                uint32_t pointsNumber,
                                const SpotLightData& lightData,
                                ImageView& shadowMapView);

      private:
        bool _fullscreenRender;
        std::optional<GraphicsPipeline> _pipeline;
        SpotLight& _light;
        SpotLightApplicator& _applicator;
      };

    private:
      bool _fullscreen(const DrawPlanBuildInfo& buildInfo) const noexcept;

    private:
      LogicalDevice& _device;
      SpotLight& _light;

      Sampler _depthMapSampler;
      Texture2D* _depthTexture;

      Sampler _normalMapSampler;
      Texture2D* _normalTexture;

      Sampler _albedoMapSampler;
      Texture2D* _albedoTexture;

      Sampler _specularMapSampler;
      Texture2D* _specularTexture;

      VolatileUniform<SpotLightData> _lightDataUniform;
      VolatileUniform<DrawMatrices> _matricesUniform;

      DrawTechnique _shapeTechnique;
      DrawTechnique _fullscreenTechnique;
    };
  }
}