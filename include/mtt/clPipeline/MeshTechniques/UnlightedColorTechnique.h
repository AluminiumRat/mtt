#pragma once

#include <mtt/render/Mesh/BaseGeometryTechnique.h>

namespace mtt
{
  namespace clPipeline
  {
    class UnlightedColorTechnique : public BaseGeometryTechnique
    {
    public:
      explicit UnlightedColorTechnique( bool depthTestEnabled,
                                        bool depthWriteEnabled,
                                        VkCompareOp depthCompareOp,
                                        VkPrimitiveTopology topology);
      UnlightedColorTechnique(const UnlightedColorTechnique&) = delete;
      UnlightedColorTechnique& operator = (
                                      const UnlightedColorTechnique&) = delete;
      virtual ~UnlightedColorTechnique()  noexcept = default;

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

    inline bool UnlightedColorTechnique::depthTestEnabled() const noexcept
    {
      return _depthTestEnabled;
    }

    inline bool UnlightedColorTechnique::depthWriteEnabled() const noexcept
    {
      return _depthWriteEnabled;
    }

    inline VkCompareOp UnlightedColorTechnique::depthCompareOp() const noexcept
    {
      return _depthCompareOp;
    }
  }
}