#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <glm/vec3.hpp>

#include <mtt/clPipeline/Lighting/AmbientLightData.h>
#include <mtt/render/Drawable/PipelineDrawable.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
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
    class AmbientLightApplicator : public DrawableNode
    {
    public:
      explicit AmbientLightApplicator(AmbientLightData& lightData,
                                      Sampler& ambientMapSampler,
                                      Sampler& diffuseLuminanceSampler,
                                      LogicalDevice& device);
      AmbientLightApplicator(const AmbientLightApplicator&) = delete;
      AmbientLightApplicator& operator = (
                                        const AmbientLightApplicator&) = delete;
      virtual ~AmbientLightApplicator() = default;

      void resetPipelines() noexcept;
      void updateBound() noexcept;

    protected:
      virtual void buildDrawActions(DrawPlanBuildInfo& buildInfo) override;

    private:
      class DrawTechnique : public PipelineDrawable
      {
      public:
        DrawTechnique(bool weightRender,
                      bool fullScreenRender,
                      AmbientLightApplicator& parent);
        DrawTechnique(const DrawTechnique&) = delete;
        DrawTechnique& operator = (const DrawTechnique&) = delete;
        virtual ~DrawTechnique() = default;

      private:
        virtual void adjustPipeline(GraphicsPipeline& pipeline) override;
        virtual void buildDrawActions(DrawPlanBuildInfo& buildInfo) override;
        void _makeApplyAction(DrawPlanBuildInfo& buildInfo,
                              DrawBin& renderBin,
                              uint32_t pointsNumber,
                              const AmbientLightDrawData& lightData);

        void _makeWeightAction( DrawPlanBuildInfo& buildInfo,
                                DrawBin& renderBin,
                                uint32_t pointsNumber,
                                const AmbientLightDrawData& lightData);
      private:
        bool _weightRender;
        bool _fullScreenRender;

        AmbientLightApplicator& _parent;
      };

    private:
      bool _fullscreen(const DrawPlanBuildInfo& buildInfo) const noexcept;

    private:
      LogicalDevice& _device;

      AmbientLightData& _lightData;

      VolatileUniform<AmbientLightDrawData> _lightDataUniform;
      VolatileUniform<DrawMatrices> _matricesUniform;

      Sampler _weightMapSampler;
      Texture2D* _weightTexture;

      Sampler _depthMapSampler;
      Texture2D* _depthTexture;

      Sampler _normalMapSampler;
      Texture2D* _normalTexture;

      Sampler _albedoMapSampler;
      Texture2D* _albedoTexture;

      Sampler _specularMapSampler;
      Texture2D* _specularTexture;

      Sampler& _ambientMapSampler;
      Sampler& _diffuseLuminanceSampler;

      struct TechniquesCollection
      {
        struct Set
        {
          std::unique_ptr<DrawTechnique> weightTechnique;
          std::unique_ptr<DrawTechnique> applyTechnique;
        };

        Set shapeSet;
        Set fullscreenSet;
      };
      TechniquesCollection _techniques;
    };
  }
}