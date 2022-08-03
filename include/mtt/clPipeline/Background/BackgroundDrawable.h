#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <glm/vec3.hpp>

#include <mtt/clPipeline/Background/BackgroundProperties.h>
#include <mtt/render/Drawable/Drawable.h>
#include <mtt/render/Drawable/PipelineDrawable.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/Pipeline/GraphicsPipeline.h>
#include <mtt/render/Pipeline/InputAttachment.h>
#include <mtt/render/Pipeline/Sampler.h>
#include <mtt/render/Pipeline/VolatileUniform.h>

namespace mtt
{
  class LogicalDevice;
  class Sampler;

  namespace clPipeline
  {
    class BackgroundDrawable : public Drawable
    {
    public:
      BackgroundDrawable( const BackgroundProperties& properties,
                          Sampler& luminanceSampler,
                          LogicalDevice& device);
      BackgroundDrawable(const BackgroundDrawable&) = delete;
      BackgroundDrawable& operator = (const BackgroundDrawable&) = delete;
      virtual ~BackgroundDrawable() = default;

      void reset() noexcept;

    protected:
      virtual void buildDrawActions(DrawPlanBuildInfo& buildInfo) override;

    private:
      class DrawTechnique : public PipelineDrawable
      {
      public:
        DrawTechnique(BackgroundDrawable& parent);
        DrawTechnique(const DrawTechnique&) = delete;
        DrawTechnique& operator = (const DrawTechnique&) = delete;
        virtual ~DrawTechnique() = default;

      protected:
        virtual void adjustPipeline(GraphicsPipeline& pipeline) override;
        virtual void buildDrawActions(DrawPlanBuildInfo& buildInfo) override;

      private:
        BackgroundDrawable& _parent;
      };

    private:
      LogicalDevice& _device;
      const BackgroundProperties& _properties;
      Sampler& _luminanceSampler;

      VolatileUniform<BackgroundDrawData> _drawDataUniform;
      VolatileUniform<DrawMatrices> _matricesUniform;
      InputAttachment _depthAttachment;

      DrawTechnique _technique;
    };
  }
}