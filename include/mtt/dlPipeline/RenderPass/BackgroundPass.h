#pragma once

#pragma once

#include <mtt/render/RenderPass/GeneralRenderPass.h>

namespace mtt
{
  class ImageView;

  namespace dlPipeline
  {
    class BackgroundPass : public GeneralRenderPass
    {
    public:
      static constexpr uint32_t colorAttachmentIndex = 0;
      static constexpr uint32_t depthMapAttachmentIndex = 1;
      static constexpr size_t attachmentNumber = 2;

    public:
      BackgroundPass( StageIndex stage,
                      VkFormat colorBufferFormat,
                      VkImageLayout colorBufferLayout,
                      VkFormat depthMapFormat,
                      VkImageLayout depthMapLayout,
                      LogicalDevice& device);
      BackgroundPass(const BackgroundPass&) = delete;
      BackgroundPass& operator = (const BackgroundPass&) = delete;
    protected:
      virtual ~BackgroundPass() noexcept = default;

    public:
      inline VkFormat colorBufferFormat() const noexcept;
      inline VkImageLayout colorBufferLayout() const noexcept;
      inline VkFormat depthMapFormat() const noexcept;
      inline VkImageLayout depthMapLayout() const noexcept;

      Ref<FrameBuffer> createFrameBuffer( ImageView& colorBuffer,
                                          ImageView& depthMap);
    private:
      VkFormat _colorBufferFormat;
      VkImageLayout _colorBufferLayout;
      VkFormat _depthMapFormat;
      VkImageLayout _depthMapLayout;
    };

    inline VkFormat BackgroundPass::colorBufferFormat() const noexcept
    {
      return _colorBufferFormat;
    }

    inline VkImageLayout BackgroundPass::colorBufferLayout() const noexcept
    {
      return _colorBufferLayout;
    }

    inline VkFormat BackgroundPass::depthMapFormat() const noexcept
    {
      return _depthMapFormat;
    }

    inline VkImageLayout BackgroundPass::depthMapLayout() const noexcept
    {
      return _depthMapLayout;
    }
  }
}