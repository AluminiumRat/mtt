#pragma once

#include <mtt/Render/RenderPass/FrameBuffer.h>
#include <mtt/Render/RenderPass/GeneralRenderPass.h>
#include <mtt/Render/Ref.h>

namespace mtt
{
  class Image;

  namespace DLPipeline
  {
    class LightingPass : public GeneralRenderPass
    {
    public:
      static constexpr size_t weightSamplerMapIndex = 0;
      static constexpr size_t depthSamplerMapIndex = 1;
      static constexpr size_t normalSamplerMapIndex = 2;
      static constexpr size_t albedoSamplerMapIndex = 3;
      static constexpr size_t specularSamplerMapIndex = 4;
      static constexpr size_t samplerMapNumber = 5;

    public:
      LightingPass( StageIndex stage,
                    VkAttachmentLoadOp colorBufferLoadOp,
                    VkFormat colorBufferFormat,
                    VkImageLayout colorBufferLayout,
                    LogicalDevice& device);
      LightingPass(const LightingPass&) = delete;
      LightingPass& operator = (const LightingPass&) = delete;
    protected:
      virtual ~LightingPass() noexcept = default;

    public:
      Ref<FrameBuffer> createFrameBuffer( ImageView& colorTarget,
                                          ImageView& weightMap,
                                          ImageView& depthMap,
                                          ImageView& normalMap,
                                          ImageView& albedoMap,
                                          ImageView& specularMap);

      inline VkAttachmentLoadOp colorBufferLoadOp() const noexcept;
      inline VkFormat colorBufferFormat() const noexcept;
      inline VkImageLayout colorBufferLayout() const noexcept;

    private:
      VkAttachmentLoadOp _colorBufferLoadOp;
      VkFormat _colorBufferFormat;
      VkImageLayout _colorBufferLayout;
    };

    inline VkAttachmentLoadOp LightingPass::colorBufferLoadOp() const noexcept
    {
      return _colorBufferLoadOp;
    }

    inline VkFormat LightingPass::colorBufferFormat() const noexcept
    {
      return _colorBufferFormat;
    }

    inline VkImageLayout LightingPass::colorBufferLayout() const noexcept
    {
      return _colorBufferLayout;
    }
  }
}