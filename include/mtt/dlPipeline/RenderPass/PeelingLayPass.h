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
    class PeelingLayPass : public AbstractRenderPass
    {
    public:
      explicit PeelingLayPass(VkFormat accumulatorFormat,
                              VkImageLayout accumulatorLayout,
                              VkFormat targetFormat,
                              VkImageLayout targetLayout,
                              LogicalDevice& device);
      PeelingLayPass(const PeelingLayPass&) = delete;
      PeelingLayPass& operator = (const PeelingLayPass&) = delete;
    protected:
      virtual ~PeelingLayPass() noexcept = default;

    public:
      inline VkFormat accumulatorFormat() const noexcept;
      inline VkImageLayout accumulatorLayout() const noexcept;
      inline VkFormat targetFormat() const noexcept;
      inline VkImageLayout targetLayout() const noexcept;

      Ref<FrameBuffer> createFrameBuffer( ImageView& targetImage,
                                          ImageView& accumulatorImage);
      void scheduleDraw(DrawContext& context);

    private:
      void _adjustRenderPass();
      void _adjustPipeline();

    private:
      VkFormat _accumulatorFormat;
      VkImageLayout _accumulatorLayout;
      VkFormat _targetFormat;
      VkImageLayout _targetLayout;

      std::optional<GraphicsPipeline> _pipeline;
      InputAttachment _accumulatorAttachment;
    };

    inline VkFormat PeelingLayPass::accumulatorFormat() const noexcept
    {
      return _accumulatorFormat;
    }

    inline VkImageLayout PeelingLayPass::accumulatorLayout() const noexcept
    {
      return _accumulatorLayout;
    }

    inline VkFormat PeelingLayPass::targetFormat() const noexcept
    {
      return _targetFormat;
    }

    inline VkImageLayout PeelingLayPass::targetLayout() const noexcept
    {
      return _targetLayout;
    }
  }
}