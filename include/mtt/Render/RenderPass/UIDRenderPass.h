#pragma once

#include <mtt/Render/RenderPass/GeneralRenderPass.h>

namespace mtt
{
  class ImageView;

  class UIDRenderPass : public GeneralRenderPass
  {
  public:
    UIDRenderPass(VkFormat uidMapFormat,
                  VkImageLayout uidMapLayout,
                  VkFormat depthBufferFormat,
                  VkImageLayout depthBufferLayout,
                  LogicalDevice& device);
    UIDRenderPass(const UIDRenderPass&) = delete;
    UIDRenderPass& operator = (const UIDRenderPass&) = delete;
  protected:
    virtual ~UIDRenderPass() = default;

  public:
    inline VkFormat uidMapFormat() const noexcept;
    inline VkImageLayout uidMapLayout() const noexcept;
    inline VkFormat depthBufferFormat() const noexcept;
    inline VkImageLayout depthBufferLayout() const noexcept;

    Ref<FrameBuffer> createFrameBuffer( ImageView& uidBuffer,
                                        ImageView& depthBuffer);
  private:
    VkFormat _uidMapFormat;
    VkImageLayout _uidMapLayout;
    VkFormat _depthBufferFormat;
    VkImageLayout _depthBufferLayout;
  };

  inline VkFormat UIDRenderPass::uidMapFormat() const noexcept
  {
    return _uidMapFormat;
  }

  inline VkImageLayout UIDRenderPass::uidMapLayout() const noexcept
  {
    return _uidMapLayout;
  }

  inline VkFormat UIDRenderPass::depthBufferFormat() const noexcept
  {
    return _depthBufferFormat;
  }

  inline VkImageLayout UIDRenderPass::depthBufferLayout() const noexcept
  {
    return _depthBufferLayout;
  }
}