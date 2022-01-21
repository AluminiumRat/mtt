#pragma once

#include <optional>

#include <mtt/Render/Mesh/BaseGeometryTechnique.h>
#include <mtt/Render/Pipeline/InputAttachment.h>

namespace mtt
{
  namespace DLPipeline
  {
    class TransparentColorTechnique : public BaseGeometryTechnique
    {
    public:
      explicit TransparentColorTechnique(VkPrimitiveTopology topology);
      TransparentColorTechnique(const TransparentColorTechnique&) = delete;
      TransparentColorTechnique& operator = (
                                    const TransparentColorTechnique&) = delete;
      virtual ~TransparentColorTechnique() noexcept = default;

      virtual void registerVariable(AbstractMeshVariable& variable,
                                    const std::string& name) override;
      virtual void unregisterVariable(
                                    AbstractMeshVariable& variable,
                                    const std::string& name) noexcept override;

    protected:
      virtual void invalidatePipeline() noexcept;

      virtual void adjustPipeline(GraphicsPipeline& pipeline,
                                  AbstractRenderPass& renderPass) override;
      virtual void createRenderAction(
                                    DrawPlanBuildInfo& buildInfo,
                                    GraphicsPipeline& pipeline,
                                    MatricesUniform& matricesUniform) override;
    private:
      std::optional<InputAttachment> _frontBufferAttachment;
      std::optional<InputAttachment> _backBufferAttachment;
      bool _useAlpha;
    };
  }
}