#pragma once

#include <mtt/render/Mesh/BaseGeometryTechnique.h>
#include <mtt/render/SceneGraph/AreaModificatorSet.h>

namespace mtt
{
  namespace clPipeline
  {
    class FLColorTechnique :  public BaseGeometryTechnique,
                              private AreaModificator::Consumer
    {
    public:
      explicit FLColorTechnique(AreaModificatorSet& modificatorSet,
                                VkPrimitiveTopology topology);
      FLColorTechnique(const FLColorTechnique&) = delete;
      FLColorTechnique& operator = (const FLColorTechnique&) = delete;
      virtual ~FLColorTechnique()  noexcept;

    protected:
      virtual void adjustPipeline(GraphicsPipeline& pipeline,
                                  AbstractRenderPass& renderPass) override;

    private:
      virtual void resetPipeline() noexcept override;

    private:
      AreaModificatorSet& _modificators;
   };
  }
}