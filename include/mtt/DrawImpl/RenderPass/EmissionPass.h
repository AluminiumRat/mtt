#pragma once

#include <mtt/Render/RenderPass/FrameBuffer.h>
#include <mtt/Render/RenderPass/GeneralRenderPass.h>
#include <mtt/Render/Ref.h>

namespace mtt
{
  class EmissionPass : public GeneralRenderPass
  {
  public:
    EmissionPass( VkAttachmentLoadOp colorBufferLoadOp,
                  VkFormat colorBufferFormat,
                  VkImageLayout colorBufferLayout,
                  VkFormat depthBufferFormat,
                  VkImageLayout depthBufferLayout,
                  LogicalDevice& device);
    EmissionPass(const EmissionPass&) = delete;
    EmissionPass& operator = (const EmissionPass&) = delete;
  protected:
    virtual ~EmissionPass() = default;

  public:
    Ref<FrameBuffer> createFrameBuffer( ImageView& colorTarget,
                                        ImageView& depthMap);

    inline VkAttachmentLoadOp colorBufferLoadOp() const noexcept;
    inline VkFormat colorBufferFormat() const noexcept;
    inline VkImageLayout colorBufferLayout() const noexcept;
    inline VkFormat depthBufferFormat() const noexcept;
    inline VkImageLayout depthBufferLayout() const noexcept;

  private:
    VkAttachmentLoadOp _colorBufferLoadOp;
    VkFormat _colorBufferFormat;
    VkImageLayout _colorBufferLayout;
    VkFormat _depthBufferFormat;
    VkImageLayout _depthBufferLayout;
  };

  inline VkAttachmentLoadOp EmissionPass::colorBufferLoadOp() const noexcept
  {
    return _colorBufferLoadOp;
  }

  inline VkFormat EmissionPass::colorBufferFormat() const noexcept
  {
    return _colorBufferFormat;
  }

  inline VkImageLayout EmissionPass::colorBufferLayout() const noexcept
  {
    return _colorBufferLayout;
  }

  inline VkFormat EmissionPass::depthBufferFormat() const noexcept
  {
    return _depthBufferFormat;
  }

  inline VkImageLayout EmissionPass::depthBufferLayout() const noexcept
  {
    return _depthBufferLayout;
  }
}