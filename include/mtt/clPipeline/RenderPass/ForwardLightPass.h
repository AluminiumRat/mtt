#pragma once

#include <mtt/render/RenderPass/FrameBuffer.h>
#include <mtt/render/RenderPass/GeneralRenderPass.h>
#include <mtt/render/Ref.h>

namespace mtt
{
  class Image;

  namespace clPipeline
  {
    class ForwardLightPass : public GeneralRenderPass
    {
    public:
      static constexpr uint32_t depthSamplerIndex = 0;
      static constexpr size_t samplersNumber = 1;

    public:
      ForwardLightPass( VkFormat colorBufferFormat,
                        VkImageLayout colorBufferLayout,
                        VkFormat depthBufferFormat,
                        VkImageLayout depthBufferLayout,
                        VkFormat depthSamplerFormat,
                        VkImageLayout depthSamplerLayout,
                        LogicalDevice& device);
      ForwardLightPass(const ForwardLightPass&) = delete;
      ForwardLightPass& operator = (const ForwardLightPass&) = delete;
    protected:
      virtual ~ForwardLightPass() noexcept = default;

    public:
      /// colorTarget should be in colorBufferFormat from constructor.
      /// depthMap should be in depthBufferFormat
      /// depthSampler should be in depthSamplerFormat
      Ref<FrameBuffer> createFrameBuffer( ImageView& colorTarget,
                                          ImageView& depthMap,
                                          ImageView& depthSampler);

      inline VkFormat colorBufferFormat() const noexcept;
      inline VkImageLayout colorBufferLayout() const noexcept;
      inline VkFormat depthBufferFormat() const noexcept;
      inline VkImageLayout depthBufferLayout() const noexcept;
      inline VkFormat depthSamplerFormat() const noexcept;
      inline VkImageLayout depthSamplerLayout() const noexcept;

    private:
      VkFormat _colorBufferFormat;
      VkImageLayout _colorBufferLayout;
      VkFormat _depthBufferFormat;
      VkImageLayout _depthBufferLayout;
      VkFormat _depthSamplerFormat;
      VkImageLayout _depthSamplerLayout;
    };

    inline VkFormat ForwardLightPass::colorBufferFormat() const noexcept
    {
      return _colorBufferFormat;
    }

    inline VkImageLayout ForwardLightPass::colorBufferLayout() const noexcept
    {
      return _colorBufferLayout;
    }

    inline VkFormat ForwardLightPass::depthBufferFormat() const noexcept
    {
      return _depthBufferFormat;
    }

    inline VkImageLayout ForwardLightPass::depthBufferLayout() const noexcept
    {
      return _depthBufferLayout;
    }

    inline VkFormat ForwardLightPass::depthSamplerFormat() const noexcept
    {
      return _depthSamplerFormat;
    }

    inline VkImageLayout ForwardLightPass::depthSamplerLayout() const noexcept
    {
      return _depthSamplerLayout;
    }
  }
}