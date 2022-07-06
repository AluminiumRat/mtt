#pragma once

#include <optional>

#include <mtt/clPipeline/Lighting/ShadowMapProvider.h>
#include <mtt/clPipeline/Lighting/PointLightData.h>
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
    class PointLight;

    class PointLightApplicator : public DrawableNode
    {
    private:
      friend class PointLight;
      PointLightApplicator(PointLight& light, LogicalDevice& device);
    public:
      PointLightApplicator(const PointLightApplicator&) = delete;
      PointLightApplicator& operator = (const PointLightApplicator&) = delete;
      virtual ~PointLightApplicator() = default;

      void resetPipelines() noexcept;
      void updateBound() noexcept;

    protected:
      virtual void buildDrawActions(DrawPlanBuildInfo& buildInfo) override;

    private:
      class DrawTechnique
      {
      public:
        DrawTechnique(bool fullscreenRender,
                      PointLight& light,
                      PointLightApplicator& applicator);
        DrawTechnique(const DrawTechnique&) = delete;
        DrawTechnique& operator = (const DrawTechnique&) = delete;
        virtual ~DrawTechnique() = default;

        void invalidatePipeline() noexcept;
        void addToDrawPlan( DrawPlanBuildInfo& buildInfo,
                            ImageView* opaqueShadowmap,
                            ImageView* transparentShadowmap);

      private:
        void _rebuildPipeline(AbstractRenderPass& renderPass);
        void _adjustPipeline();

        void _makeNonshadowCommand( DrawPlanBuildInfo& buildInfo,
                                    uint32_t pointsNumber,
                                    const PointLightData& lightData);

        void _makeShadowCommand(DrawPlanBuildInfo& buildInfo,
                                uint32_t pointsNumber,
                                const PointLightData& lightData,
                                ImageView& opaqueShadowmap,
                                ImageView& transparentShadowmap);

      private:
        bool _fullscreenRender;
        std::optional<GraphicsPipeline> _pipeline;
        PointLight& _light;
        PointLightApplicator& _applicator;
      };

    private:
      bool _fullscreen(const DrawPlanBuildInfo& buildInfo) const noexcept;

    private:
      LogicalDevice& _device;
      PointLight& _light;

      Sampler _depthMapSampler;
      Texture2D* _depthTexture;

      Sampler _normalMapSampler;
      Texture2D* _normalTexture;

      Sampler _albedoMapSampler;
      Texture2D* _albedoTexture;

      Sampler _specularMapSampler;
      Texture2D* _specularTexture;

      VolatileUniform<PointLightData> _lightDataUniform;
      VolatileUniform<DrawMatrices> _matricesUniform;

      DrawTechnique _shapeTechnique;
      DrawTechnique _fullscreenTechnique;
    };
  }
}