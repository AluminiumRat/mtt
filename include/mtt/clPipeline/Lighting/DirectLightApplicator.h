#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <mtt/clPipeline/Lighting/CascadeShadowMapProvider.h>
#include <mtt/clPipeline/Lighting/DirectLightData.h>
#include <mtt/render/Drawable/PipelineDrawable.h>
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
    class DirectLight;

    class DirectLightApplicator : public DrawableNode
    {
    private:
      friend class DirectLight;
      DirectLightApplicator(DirectLight& light, LogicalDevice& device);

    public:
      DirectLightApplicator(const DirectLightApplicator&) = delete;
      DirectLightApplicator& operator = (const DirectLightApplicator&) = delete;
      virtual ~DirectLightApplicator() = default;

      void resetPipelines() noexcept;

    protected:
      virtual void buildDrawActions(DrawPlanBuildInfo& buildInfo) override;

    private:
      class DrawTechnique : public PipelineDrawable
      {
      public:
        DrawTechnique(DirectLight& light,
                      DirectLightApplicator& applicator);
        DrawTechnique(const DrawTechnique&) = delete;
        DrawTechnique& operator = (const DrawTechnique&) = delete;
        virtual ~DrawTechnique() = default;

      protected:
        virtual void adjustPipeline(GraphicsPipeline& pipeline) override;
        virtual void buildDrawActions(DrawPlanBuildInfo& buildInfo) override;

      private:
        void _makeNonshadowCommand( DrawPlanBuildInfo& buildInfo,
                                    const DirectLightData& lightData);

        void _makeShadowCommand(
                      DrawPlanBuildInfo& buildInfo,
                      const DirectLightData& lightData,
                      const CascadeShadowMapProvider::CascadeInfo& cascadeInfo);

      private:
        DirectLight& _light;
        DirectLightApplicator& _applicator;
      };

    private:
      LogicalDevice& _device;
      DirectLight& _light;

      Sampler _depthMapSampler;
      Texture2D* _depthTexture;

      Sampler _normalMapSampler;
      Texture2D* _normalTexture;

      Sampler _albedoMapSampler;
      Texture2D* _albedoTexture;

      Sampler _specularMapSampler;
      Texture2D* _specularTexture;

      VolatileUniform<DirectLightData> _lightDataUniform;

      using CoordsCorrectionData = std::vector<glm::vec4>;
      VolatileUniform<CoordsCorrectionData> _coordsCorrectionUniform;

      DrawTechnique _technique;
    };
  }
}