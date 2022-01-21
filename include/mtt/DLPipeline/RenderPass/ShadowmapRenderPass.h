#pragma once

#include <mtt/Render/RenderPass/GeneralRenderPass.h>

namespace mtt
{
  class ImageView;

  namespace DLPipeline
  {
    class ShadowmapRenderPass : public GeneralRenderPass
    {
    public:
      inline static const size_t attachmentNumber = 2;
      inline static const uint32_t shadowmapAttachmentIndex = 0;
      inline static const uint32_t depthAttachmentIndex = 1;

    public:
      ShadowmapRenderPass(VkFormat shadowmapFormat,
                          VkImageLayout shadowmapLayout,
                          VkFormat depthmapFormat,
                          VkImageLayout depthmapLayout,
                          LogicalDevice& device);
      ShadowmapRenderPass(const ShadowmapRenderPass&) = delete;
      ShadowmapRenderPass& operator = (const ShadowmapRenderPass&) = delete;
    protected:
      virtual ~ShadowmapRenderPass() = default;

    public:
      inline VkFormat shadowmapFormat() const noexcept;
      inline VkImageLayout shadowmapLayout() const noexcept;
      inline VkFormat depthmapFormat() const noexcept;
      inline VkImageLayout depthmapLayout() const noexcept;

      Ref<FrameBuffer> createFrameBuffer( ImageView& shadowMapBuffer,
                                          ImageView& depthBuffer);
    private:
      VkFormat _shadowmapFormat;
      VkImageLayout _shadowmapLayout;
      VkFormat _depthmapFormat;
      VkImageLayout _depthmapLayout;
    };

    inline VkFormat ShadowmapRenderPass::shadowmapFormat() const noexcept
    {
      return _shadowmapFormat;
    }

    inline VkImageLayout ShadowmapRenderPass::shadowmapLayout() const noexcept
    {
      return _shadowmapLayout;
    }

    inline VkFormat ShadowmapRenderPass::depthmapFormat() const noexcept
    {
      return _depthmapFormat;
    }

    inline VkImageLayout ShadowmapRenderPass::depthmapLayout() const noexcept
    {
      return _depthmapLayout;
    }
  }
}