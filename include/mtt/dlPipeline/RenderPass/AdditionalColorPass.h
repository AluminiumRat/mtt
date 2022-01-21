#pragma once

#include <mtt/render/RenderPass/FrameBuffer.h>
#include <mtt/render/RenderPass/GeneralRenderPass.h>
#include <mtt/render/Ref.h>

namespace mtt
{
  class Image;

  namespace dlPipeline
  {
    class AdditionalColorPass : public GeneralRenderPass
    {
    public:
      AdditionalColorPass(VkFormat colorBufferFormat,
                          VkImageLayout colorBufferLayout,
                          VkFormat depthBufferFormat,
                          VkImageLayout depthBufferLayout,
                          StageIndex stage,
                          LogicalDevice& device);
      AdditionalColorPass(const AdditionalColorPass&) = delete;
      AdditionalColorPass& operator = (const AdditionalColorPass&) = delete;
    protected:
      virtual ~AdditionalColorPass() noexcept = default;

    public:
      /// colorTarget should be in colorBufferFormat from constructor.
      /// depthMap should be in depthBufferFormat
      Ref<FrameBuffer> createFrameBuffer( ImageView& colorTarget,
                                          ImageView& depthMap);

      inline VkFormat colorBufferFormat() const noexcept;
      inline VkImageLayout colorBufferLayout() const noexcept;
      inline VkFormat depthBufferFormat() const noexcept;
      inline VkImageLayout depthBufferLayout() const noexcept;
      inline StageIndex stage() const noexcept;

    private:
      VkFormat _colorBufferFormat;
      VkImageLayout _colorBufferLayout;
      VkFormat _depthBufferFormat;
      VkImageLayout _depthBufferLayout;
      StageIndex _stage;
    };

    inline VkFormat AdditionalColorPass::colorBufferFormat() const noexcept
    {
      return _colorBufferFormat;
    }

    inline VkImageLayout AdditionalColorPass::colorBufferLayout() const noexcept
    {
      return _colorBufferLayout;
    }

    inline VkFormat AdditionalColorPass::depthBufferFormat() const noexcept
    {
      return _depthBufferFormat;
    }

    inline VkImageLayout AdditionalColorPass::depthBufferLayout() const noexcept
    {
      return _depthBufferLayout;
    }

    inline StageIndex AdditionalColorPass::stage() const noexcept
    {
      return _stage;
    }
  }
}