#pragma once

#include <mtt/Render/RenderPass/GeneralRenderPass.h>

namespace mtt
{
  class ImageView;

  namespace DLPipeline
  {
    class GBufferPass : public GeneralRenderPass
    {
    public:
      GBufferPass(VkFormat depthMapFormat,
                  VkImageLayout depthMapLayout,
                  VkFormat normalMapFormat,
                  VkImageLayout normalMapLayout,
                  VkFormat albedoMapFormat,
                  VkImageLayout albedoMapLayout,
                  VkFormat specularMapFormat,
                  VkImageLayout specularMapLayout,
                  LogicalDevice& device);
      GBufferPass(const GBufferPass&) = delete;
      GBufferPass& operator = (const GBufferPass&) = delete;
    protected:
      virtual ~GBufferPass() noexcept = default;

    public:
      inline VkFormat depthMapFormat() const noexcept;
      inline VkImageLayout depthMapLayout() const noexcept;
      inline VkFormat normalMapFormat() const noexcept;
      inline VkImageLayout normalMapLayout() const noexcept;
      inline VkFormat albedoMapFormat() const noexcept;
      inline VkImageLayout albedoMapLayout() const noexcept;
      inline VkFormat specularMapFormat() const noexcept;
      inline VkImageLayout specularMapLayout() const noexcept;

      Ref<FrameBuffer> createFrameBuffer( ImageView& depthMap,
                                          ImageView& normalMap,
                                          ImageView& albedoMap,
                                          ImageView& specularMap);
    private:
      VkFormat _depthMapFormat;
      VkImageLayout _depthMapLayout;
      VkFormat _normalMapFormat;
      VkImageLayout _normalMapLayout;
      VkFormat _albedoMapFormat;
      VkImageLayout _albedoMapLayout;
      VkFormat _specularMapFormat;
      VkImageLayout _specularMapLayout;
    };

    inline VkFormat GBufferPass::depthMapFormat() const noexcept
    {
      return _depthMapFormat;
    }

    inline VkImageLayout GBufferPass::depthMapLayout() const noexcept
    {
      return _depthMapLayout;
    }

    inline VkFormat GBufferPass::normalMapFormat() const noexcept
    {
      return _normalMapFormat;
    }

    inline VkImageLayout GBufferPass::normalMapLayout() const noexcept
    {
      return _normalMapLayout;
    }

    inline VkFormat GBufferPass::albedoMapFormat() const noexcept
    {
      return _albedoMapFormat;
    }

    inline VkImageLayout GBufferPass::albedoMapLayout() const noexcept
    {
      return _albedoMapLayout;
    }

    inline VkFormat GBufferPass::specularMapFormat() const noexcept
    {
      return _specularMapFormat;
    }

    inline VkImageLayout GBufferPass::specularMapLayout() const noexcept
    {
      return _specularMapLayout;
    }
  }
}