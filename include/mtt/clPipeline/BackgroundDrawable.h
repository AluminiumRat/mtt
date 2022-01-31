#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <glm/vec3.hpp>

#include <mtt/render/Drawable/Drawable.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/Pipeline/CubeTexture.h>
#include <mtt/render/Pipeline/GraphicsPipeline.h>
#include <mtt/render/Pipeline/InputAttachment.h>
#include <mtt/render/Pipeline/Sampler.h>
#include <mtt/render/Pipeline/VolatileUniform.h>

namespace mtt
{
  class LogicalDevice;

  namespace clPipeline
  {
    class BackgroundDrawable : public Drawable
    {
    public:
      explicit BackgroundDrawable(LogicalDevice& device);
      BackgroundDrawable(const BackgroundDrawable&) = delete;
      BackgroundDrawable& operator = (const BackgroundDrawable&) = delete;
      virtual ~BackgroundDrawable() = default;

      inline const glm::vec3& luminance() const noexcept;
      inline void setLuminance(const glm::vec3& newValue) noexcept;

      inline float dissolutionStartDistance() const noexcept;
      inline void setDissolutionStartDistance(float newValue) noexcept;

      inline float dissolutionLength() const noexcept;
      inline void setDissolutionLength(float newValue) noexcept;

      void setLuminanceTexture(std::shared_ptr<CubeTexture> texture) noexcept;
      inline const CubeTexture* luminanceTexture() const noexcept;
      inline CubeTexture* luminanceTexture() noexcept;

    protected:
      virtual void buildDrawActions(DrawPlanBuildInfo& buildInfo) override;

    private:
      struct Properties
      {
        alignas(16) glm::vec3 luminance;
        alignas(4) float dissolutionStartDistance;
        alignas(4) float dissolutionLength;
      };

      struct DrawData
      {
        Properties properties;
        alignas(16) glm::mat4 viewToLocal;
      };

      class DrawTechnique
      {
      public:
        DrawTechnique(BackgroundDrawable& parent);
        DrawTechnique(const DrawTechnique&) = delete;
        DrawTechnique& operator = (const DrawTechnique&) = delete;
        virtual ~DrawTechnique() = default;
  
        void addToDrawPlan(DrawPlanBuildInfo& buildInfo);
        void invalidate() noexcept;

      private:
        void _rebuildPipeline(AbstractRenderPass& renderPass, StageIndex stage);

      private:
        std::optional<GraphicsPipeline> _pipeline;
        BackgroundDrawable& _parent;
      };

    private:
      LogicalDevice& _device;

      Properties _properties;

      VolatileUniform<DrawData> _drawDataUniform;
      VolatileUniform<DrawMatrices> _matricesUniform;
      InputAttachment _depthAttachment;
      Sampler _luminanceSampler;

      DrawTechnique _technique;
    };

    inline const glm::vec3& BackgroundDrawable::luminance() const noexcept
    {
      return _properties.luminance;
    }

    inline void BackgroundDrawable::setLuminance(
                                            const glm::vec3& newValue) noexcept
    {
      _properties.luminance = newValue;
    }

    inline float BackgroundDrawable::dissolutionStartDistance() const noexcept
    {
      return _properties.dissolutionStartDistance;
    }

    inline void BackgroundDrawable::setDissolutionStartDistance(
                                                        float newValue) noexcept
    {
      _properties.dissolutionStartDistance = newValue;
    }

    inline float BackgroundDrawable::dissolutionLength() const noexcept
    {
      return _properties.dissolutionLength;
    }

    inline void BackgroundDrawable::setDissolutionLength(
                                                        float newValue) noexcept
    {
      _properties.dissolutionLength = newValue;
    }

    inline const CubeTexture*
                          BackgroundDrawable::luminanceTexture() const noexcept
    {
      return static_cast<const CubeTexture*>(
                                          _luminanceSampler.attachedTexture(0));
    }

    inline CubeTexture* BackgroundDrawable::luminanceTexture() noexcept
    {
      return static_cast<CubeTexture*>(_luminanceSampler.attachedTexture(0));
    }
  }
}