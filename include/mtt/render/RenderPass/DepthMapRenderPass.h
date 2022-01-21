#pragma once

#include <mtt/render/RenderPass/GeneralRenderPass.h>

namespace mtt
{
  class ImageView;

  class DepthMapRenderPass : public GeneralRenderPass
  {
  public:
    DepthMapRenderPass( VkFormat depthMapFormat,
                        VkImageLayout depthMapLayout,
                        LogicalDevice& device);
    DepthMapRenderPass(const DepthMapRenderPass&) = delete;
    DepthMapRenderPass& operator = (const DepthMapRenderPass&) = delete;
  protected:
    virtual ~DepthMapRenderPass() = default;

  public:
    inline VkFormat depthMapFormat() const noexcept;
    inline VkImageLayout depthMapLayout() const noexcept;

    /// The image should have a primary layout equal to depthMapLayout
    /// of this pass
    Ref<FrameBuffer> createFrameBuffer(ImageView& depthBuffer);

  private:
    VkFormat _depthMapFormat;
    VkImageLayout _depthMapLayout;
  };

  inline VkFormat DepthMapRenderPass::depthMapFormat() const noexcept
  {
    return _depthMapFormat;
  }

  inline VkImageLayout DepthMapRenderPass::depthMapLayout() const noexcept
  {
    return _depthMapLayout;
  }
}