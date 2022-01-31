#pragma once

#include <mtt/render/Mesh/BaseGeometryTechnique.h>

namespace mtt
{
  namespace clPipeline
  {
    class TransparentColorTechnique : public BaseGeometryTechnique
    {
    public:
      explicit TransparentColorTechnique( bool depthTestEnabled,
                                          bool depthWriteEnabled,
                                          VkCompareOp depthCompareOp,
                                          VkPrimitiveTopology topology);
      TransparentColorTechnique(const TransparentColorTechnique&) = delete;
      TransparentColorTechnique& operator = (
                                    const TransparentColorTechnique&) = delete;
      virtual ~TransparentColorTechnique()  noexcept = default;

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

    inline bool TransparentColorTechnique::depthTestEnabled() const noexcept
    {
      return _depthTestEnabled;
    }

    inline bool TransparentColorTechnique::depthWriteEnabled() const noexcept
    {
      return _depthWriteEnabled;
    }

    inline VkCompareOp
                      TransparentColorTechnique::depthCompareOp() const noexcept
    {
      return _depthCompareOp;
    }
  }
}