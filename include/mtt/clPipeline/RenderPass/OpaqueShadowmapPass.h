#pragma once

#include <mtt/render/RenderPass/GeneralRenderPass.h>

namespace mtt
{
  class ImageView;

  namespace clPipeline
  {
    class OpaqueShadowmapPass : public GeneralRenderPass
    {
    public:
      inline static const size_t attachmentNumber = 2;
      inline static const uint32_t shadowmapAttachmentIndex = 0;
      inline static const uint32_t depthAttachmentIndex = 1;

    public:
      OpaqueShadowmapPass(VkFormat shadowmapFormat,
                          VkImageLayout shadowmapLayout,
                          VkFormat depthmapFormat,
                          VkImageLayout depthmapLayout,
                          LogicalDevice& device);
      OpaqueShadowmapPass(const OpaqueShadowmapPass&) = delete;
      OpaqueShadowmapPass& operator = (const OpaqueShadowmapPass&) = delete;
    protected:
      virtual ~OpaqueShadowmapPass() = default;

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

    inline VkFormat OpaqueShadowmapPass::shadowmapFormat() const noexcept
    {
      return _shadowmapFormat;
    }

    inline VkImageLayout OpaqueShadowmapPass::shadowmapLayout() const noexcept
    {
      return _shadowmapLayout;
    }

    inline VkFormat OpaqueShadowmapPass::depthmapFormat() const noexcept
    {
      return _depthmapFormat;
    }

    inline VkImageLayout OpaqueShadowmapPass::depthmapLayout() const noexcept
    {
      return _depthmapLayout;
    }
  }
}