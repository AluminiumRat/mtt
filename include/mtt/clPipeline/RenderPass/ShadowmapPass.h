#pragma once

#include <mtt/render/RenderPass/GeneralRenderPass.h>

namespace mtt
{
  class ImageView;

  namespace clPipeline
  {
    class ShadowmapPass : public GeneralRenderPass
    {
    public:
      inline static const size_t attachmentNumber = 2;
      inline static const uint32_t shadowmapAttachmentIndex = 0;
      inline static const uint32_t depthAttachmentIndex = 1;

    public:
      ShadowmapPass(VkFormat shadowmapFormat,
                    VkImageLayout shadowmapLayout,
                    VkFormat depthmapFormat,
                    VkImageLayout depthmapLayout,
                    mtt::StageIndex stageIndex,
                    LogicalDevice& device);
      ShadowmapPass(const ShadowmapPass&) = delete;
      ShadowmapPass& operator = (const ShadowmapPass&) = delete;
    protected:
      virtual ~ShadowmapPass() = default;

    public:
      inline VkFormat shadowmapFormat() const noexcept;
      inline VkImageLayout shadowmapLayout() const noexcept;
      inline VkFormat depthmapFormat() const noexcept;
      inline VkImageLayout depthmapLayout() const noexcept;

      inline mtt::StageIndex stageIndex() const noexcept;

      Ref<FrameBuffer> createFrameBuffer( ImageView& shadowMapBuffer,
                                          ImageView& depthBuffer);
    private:
      VkFormat _shadowmapFormat;
      VkImageLayout _shadowmapLayout;
      VkFormat _depthmapFormat;
      VkImageLayout _depthmapLayout;
      mtt::StageIndex _stageIndex;
    };

    inline VkFormat ShadowmapPass::shadowmapFormat() const noexcept
    {
      return _shadowmapFormat;
    }

    inline VkImageLayout ShadowmapPass::shadowmapLayout() const noexcept
    {
      return _shadowmapLayout;
    }

    inline VkFormat ShadowmapPass::depthmapFormat() const noexcept
    {
      return _depthmapFormat;
    }

    inline VkImageLayout ShadowmapPass::depthmapLayout() const noexcept
    {
      return _depthmapLayout;
    }

    inline mtt::StageIndex ShadowmapPass::stageIndex() const noexcept
    {
      return _stageIndex;
    }
  }
}