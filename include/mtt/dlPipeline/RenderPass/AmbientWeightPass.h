#pragma once

#include <mtt/render/RenderPass/FrameBuffer.h>
#include <mtt/render/RenderPass/GeneralRenderPass.h>
#include <mtt/render/Ref.h>

namespace mtt
{
  namespace dlPipeline
  {
    class AmbientWeightPass : public GeneralRenderPass
    {
    public:
      static constexpr size_t depthSamplerMapIndex = 0;
      static constexpr size_t samplerMapNumber = 1;

    public:
      AmbientWeightPass(StageIndex stage,
                        VkFormat weightBufferFormat,
                        VkImageLayout weightBufferLayout,
                        LogicalDevice& device);
      AmbientWeightPass(const AmbientWeightPass&) = delete;
      AmbientWeightPass& operator = (const AmbientWeightPass&) = delete;
    protected:
      virtual ~AmbientWeightPass() = default;

    public:
      Ref<FrameBuffer> createFrameBuffer( ImageView& weightTarget,
                                          ImageView& depthMap);

      inline VkFormat weightBufferFormat() const noexcept;
      inline VkImageLayout weightBufferLayout() const noexcept;

    private:
      VkFormat _weightBufferFormat;
      VkImageLayout _weightBufferLayout;
    };

    inline VkFormat AmbientWeightPass::weightBufferFormat() const noexcept
    {
      return _weightBufferFormat;
    }

    inline VkImageLayout AmbientWeightPass::weightBufferLayout() const noexcept
    {
      return _weightBufferLayout;
    }
  }
}