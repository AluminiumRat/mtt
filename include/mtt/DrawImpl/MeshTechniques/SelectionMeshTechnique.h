#pragma once

#include <mtt/Render/Mesh/BaseGeometryTechnique.h>

namespace mtt
{
  class SelectionMeshTechnique : public BaseGeometryTechnique
  {
  public:
    explicit SelectionMeshTechnique(VkPrimitiveTopology topology);
    SelectionMeshTechnique(const SelectionMeshTechnique&) = delete;
    SelectionMeshTechnique& operator = (const SelectionMeshTechnique&) = delete;
    virtual ~SelectionMeshTechnique()  noexcept = default;

  protected:
    virtual void adjustPipeline(GraphicsPipeline& pipeline,
                                AbstractRenderPass& renderPass) override;
  };
}