#pragma once

#include <mtt/render/RenderPass/GeneralRenderPass.h>

namespace mtt
{
  class ImageView;

  namespace clPipeline
  {
    class TransparentShadowmapPass : public GeneralRenderPass
    {
    public:
      inline static const size_t attachmentNumber = 1;
      inline static const uint32_t shadowmapAttachmentIndex = 0;

    public:
      TransparentShadowmapPass( VkFormat shadowmapFormat,
                                VkImageLayout shadowmapLayout,
                                LogicalDevice& device);
      TransparentShadowmapPass(const TransparentShadowmapPass&) = delete;
      TransparentShadowmapPass& operator = (
                                        const TransparentShadowmapPass&) = delete;
    protected:
      virtual ~TransparentShadowmapPass() = default;

    public:
      inline VkFormat shadowmapFormat() const noexcept;
      inline VkImageLayout shadowmapLayout() const noexcept;

      Ref<FrameBuffer> createFrameBuffer( ImageView& shadowMapBuffer);
    private:
      VkFormat _shadowmapFormat;
      VkImageLayout _shadowmapLayout;
    };

    inline VkFormat TransparentShadowmapPass::shadowmapFormat() const noexcept
    {
      return _shadowmapFormat;
    }

    inline VkImageLayout TransparentShadowmapPass::shadowmapLayout() const noexcept
    {
      return _shadowmapLayout;
    }
  }
}