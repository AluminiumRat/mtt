#pragma once

#include <optional>

#include <mtt/render/Pipeline/GraphicsPipeline.h>
#include <mtt/render/Pipeline/InputAttachment.h>
#include <mtt/render/RenderPass/AbstractRenderPass.h>

namespace mtt
{
  class ImageView;

  namespace dlPipeline
  {
    class PeelingMergePass : public AbstractRenderPass
    {
    public:
      explicit PeelingMergePass(VkFormat layerFormat,
                                VkImageLayout layerLayout,
                                VkFormat accumulatorFormat,
                                VkImageLayout accumulatorLayout,
                                LogicalDevice& device);
      PeelingMergePass(const PeelingMergePass&) = delete;
      PeelingMergePass& operator = (const PeelingMergePass&) = delete;
    protected:
      virtual ~PeelingMergePass() noexcept = default;

    public:
      inline VkFormat layerFormat() const noexcept;
      inline VkImageLayout layerLayout() const noexcept;
      inline VkFormat accumulatorFormat() const noexcept;
      inline VkImageLayout accumulatorLayout() const noexcept;

      Ref<FrameBuffer> createFrameBuffer( ImageView& accumulatorImage,
                                          ImageView& layerImage);
      void scheduleDraw(DrawContext& context);

    private:
      void _adjustRenderPass();
      void _adjustPipeline();

    private:
      VkFormat _layerFormat;
      VkImageLayout _layerLayout;
      VkFormat _accumulatorFormat;
      VkImageLayout _accumulatorLayout;

      std::optional<GraphicsPipeline> _pipeline;
      InputAttachment _layerAttachment;
    };

    inline VkFormat PeelingMergePass::layerFormat() const noexcept
    {
      return _layerFormat;
    }

    inline VkImageLayout PeelingMergePass::layerLayout() const noexcept
    {
      return _layerLayout;
    }

    inline VkFormat PeelingMergePass::accumulatorFormat() const noexcept
    {
      return _accumulatorFormat;
    }

    inline VkImageLayout PeelingMergePass::accumulatorLayout() const noexcept
    {
      return _accumulatorLayout;
    }
  }
}