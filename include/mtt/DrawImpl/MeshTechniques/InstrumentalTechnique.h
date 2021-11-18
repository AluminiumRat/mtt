#pragma once

#include <mtt/Render/Mesh/BaseGeometryTechnique.h>

namespace mtt
{
  class InstrumentalTechnique : public BaseGeometryTechnique
  {
  public:
    explicit InstrumentalTechnique( bool depthTestEnabled,
                                    bool depthWriteEnabled,
                                    VkCompareOp depthCompareOp,
                                    VkPrimitiveTopology topology);
    InstrumentalTechnique(const InstrumentalTechnique&) = delete;
    InstrumentalTechnique& operator = (const InstrumentalTechnique&) = delete;
    virtual ~InstrumentalTechnique()  noexcept = default;

    inline bool depthTestEnabled() const noexcept;
    inline bool depthWriteEnabled() const noexcept;
    inline VkCompareOp depthCompareOp() const noexcept;

  protected:
    virtual void adjustPipeline(GraphicsPipeline& pipeline,
                                AbstractRenderPass& renderPass) override;
  private:
    bool _depthTestEnabled;
    bool _depthWriteEnabled;
    VkCompareOp _depthCompareOp;
 };

  inline bool InstrumentalTechnique::depthTestEnabled() const noexcept
  {
    return _depthTestEnabled;
  }

  inline bool InstrumentalTechnique::depthWriteEnabled() const noexcept
  {
    return _depthWriteEnabled;
  }

  inline VkCompareOp InstrumentalTechnique::depthCompareOp() const noexcept
  {
    return _depthCompareOp;
  }
}