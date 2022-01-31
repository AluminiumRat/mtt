#pragma once

#include <mtt/render/RenderPass/FrameBuffer.h>
#include <mtt/render/RenderPass/GeneralRenderPass.h>
#include <mtt/render/Ref.h>

namespace mtt
{
  class Image;

  namespace clPipeline
  {
    class SelectionPass : public GeneralRenderPass
    {
    public:
      SelectionPass(VkFormat colorBufferFormat,
                    VkImageLayout colorBufferLayout,
                    VkFormat depthBufferFormat,
                    VkImageLayout depthBufferLayout,
                    LogicalDevice& device);
      SelectionPass(const SelectionPass&) = delete;
      SelectionPass& operator = (const SelectionPass&) = delete;
    protected:
      virtual ~SelectionPass() noexcept = default;

    public:
      /// colorTarget should be in colorBufferFormat from constructor.
      /// depthMap should be in depthBufferFormat
      Ref<FrameBuffer> createFrameBuffer( ImageView& colorTarget,
                                          ImageView& depthMap);

      inline VkFormat colorBufferFormat() const noexcept;
      inline VkImageLayout colorBufferLayout() const noexcept;
      inline VkFormat depthBufferFormat() const noexcept;
      inline VkImageLayout depthBufferLayout() const noexcept;

    private:
      VkFormat _colorBufferFormat;
      VkImageLayout _colorBufferLayout;
      VkFormat _depthBufferFormat;
      VkImageLayout _depthBufferLayout;
    };

    inline VkFormat SelectionPass::colorBufferFormat() const noexcept
    {
      return _colorBufferFormat;
    }

    inline VkImageLayout SelectionPass::colorBufferLayout() const noexcept
    {
      return _colorBufferLayout;
    }

    inline VkFormat SelectionPass::depthBufferFormat() const noexcept
    {
      return _depthBufferFormat;
    }

    inline VkImageLayout SelectionPass::depthBufferLayout() const noexcept
    {
      return _depthBufferLayout;
    }
  }
}