#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <glm/vec3.hpp>

#include <mtt/dlPipeline/Lighting/AbstractLightApplicator.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/Pipeline/GraphicsPipeline.h>
#include <mtt/render/Pipeline/Sampler.h>
#include <mtt/render/Pipeline/CubeTexture.h>
#include <mtt/render/Pipeline/Texture2D.h>
#include <mtt/render/Pipeline/VolatileUniform.h>

namespace mtt
{
  class LogicalDevice;

  namespace dlPipeline
  {
    class AmbientLightApplicator : public AbstractLightApplicator
    {
    public:
      explicit AmbientLightApplicator(LogicalDevice& device);
      AmbientLightApplicator(const AmbientLightApplicator&) = delete;
      AmbientLightApplicator& operator = (
                                        const AmbientLightApplicator&) = delete;
      virtual ~AmbientLightApplicator() = default;

      inline bool infinityAreaMode() const noexcept;
      inline void setInfinityAreaMode(bool newValue) noexcept;

      inline float saturationDistance() const noexcept;
      inline void setSaturationDistance(float newValue) noexcept;

      inline CubeTexture* ambientMap() const noexcept;
      void setAmbientMap(std::shared_ptr<CubeTexture> newMap);

      inline CubeTexture* diffuseLuminanceMap() const noexcept;
      void setDiffuseLuminanceMap(std::shared_ptr<CubeTexture> newMap);

    protected:
      virtual void buildDrawActions(DrawPlanBuildInfo& buildInfo) override;

    private:
      struct LightData
      {
        alignas(16) glm::vec3 illuminance;
        alignas(16) glm::vec3 position;
        alignas(4) float distance;
        alignas(4) float saturationDistance;
        alignas(16) glm::mat4 clipToView;
        alignas(16) glm::mat4 viewToLocal;
      };

      class DrawTechnique
      {
      public:
        DrawTechnique(bool weightRender,
                      bool transparentRender,
                      bool fullScreenRender,
                      AmbientLightApplicator& parent);
        DrawTechnique(const DrawTechnique&) = delete;
        DrawTechnique& operator = (const DrawTechnique&) = delete;
        virtual ~DrawTechnique() = default;
  
        void invalidate() noexcept;

        void addToDrawPlan(DrawPlanBuildInfo& buildInfo);

      private:
        void _rebuildPipeline(AbstractRenderPass& renderPass, StageIndex stage);
        void _adjustPipeline(GraphicsPipeline& pipeline);

        void _makeApplyAction(DrawPlanBuildInfo& buildInfo,
                              DrawBin& renderBin,
                              uint32_t pointsNumber,
                              const LightData& lightData);

        void _makeWeightAction( DrawPlanBuildInfo& buildInfo,
                                DrawBin& renderBin,
                                uint32_t pointsNumber,
                                const LightData& lightData);
      private:
        std::optional<GraphicsPipeline> _pipeline;

        bool _weightRender;
        bool _transparentRender;
        bool _fullScreenRender;

        AmbientLightApplicator& _parent;
      };

    private:
      bool _fullscreen(const DrawPlanBuildInfo& buildInfo) const noexcept;
      void _invalidateTechniques() noexcept;

    private:
      LogicalDevice& _device;

      bool _infinityAreaMode;
      float _saturationDistance;

      VolatileUniform<LightData> _lightDataUniform;
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

      Sampler _ambientMapSampler;
      CubeTexture* _ambientMap;

      Sampler _luminanceMapSampler;
      CubeTexture* _diffuseLuminanceMap;

      struct TechniquesCollection
      {
        struct Set
        {
          std::unique_ptr<DrawTechnique> weightTechnique;
          std::unique_ptr<DrawTechnique> applyTechnique;

          std::unique_ptr<DrawTechnique> peelingWeightTechnique;
          std::unique_ptr<DrawTechnique> peelingApplyTechnique;
        };

        Set shapeSet;
        Set fullscreenSet;
      };
      TechniquesCollection _techniques;
    };

    inline bool AmbientLightApplicator::infinityAreaMode() const noexcept
    {
      return _infinityAreaMode;
    }

    inline void AmbientLightApplicator::setInfinityAreaMode(
                                                        bool newValue) noexcept
    {
      if(_infinityAreaMode == newValue) return;
      _infinityAreaMode = newValue;
      _invalidateTechniques();
    }

    inline float AmbientLightApplicator::saturationDistance() const noexcept
    {
      return _saturationDistance;
    }

    inline void AmbientLightApplicator::setSaturationDistance(
                                                        float newValue) noexcept
    {
      _saturationDistance = newValue;
    }

    inline CubeTexture* AmbientLightApplicator::ambientMap() const noexcept
    {
      return _ambientMap;
    }

    inline CubeTexture*
                    AmbientLightApplicator::diffuseLuminanceMap() const noexcept
    {
      return _diffuseLuminanceMap;
    }
  }
}