#pragma once

#include <memory>

#include <mtt/Render/RenderPass/UIDRenderPass.h>
#include <mtt/Render/SceneRenderer/AbstractFrame.h>
#include <mtt/Render/SceneRenderer/OneTargetFrameBuilder.h>
#include <mtt/Render/Ref.h>

namespace mtt
{
  class Image;

  /// Target image in VK_IMAGE_ASPECT_COLOR_BIT aspect.
  class UidFrameBuilder : public OneTargetFrameBuilder
  {
  public:
    UidFrameBuilder(VkFormat uidMapFormat,
                    VkImageLayout uidMapLayout,
                    VkImageUsageFlags uidMapUsage,
                    LogicalDevice& device);
    UidFrameBuilder(const UidFrameBuilder&) = delete;
    UidFrameBuilder& operator = (const UidFrameBuilder&) = delete;
    virtual ~UidFrameBuilder() noexcept = default;

    /// Target image in UIDRenderPass::uidFormat. R component is low part of
    /// uid, G component is high part.
    virtual std::unique_ptr<Frame> createFrame(
                                            const glm::uvec2& extent) override;

    /// The frame should be created by this frame renderer
    virtual std::unique_ptr<AbstractFramePlan> createFramePlan(
                                          AbstractFrame& frame) const override;

    inline VkFormat uidMapFormat() const noexcept;
    inline VkImageLayout uidMapLayout() const noexcept;
    inline VkImageUsageFlags uidMapUsage() const noexcept;

    virtual void scheduleRender(AbstractFramePlan& plan,
                                CommandProducer& drawProducer) override;
  private:
    VkImageUsageFlags _uidMapUsage;

    Ref<UIDRenderPass> _renderPass;
    LogicalDevice& _device;
  };

  inline VkFormat UidFrameBuilder::uidMapFormat() const noexcept
  {
    return _renderPass->uidMapFormat();
  }

  inline VkImageLayout UidFrameBuilder::uidMapLayout() const noexcept
  {
    return _renderPass->uidMapLayout();
  }

  inline VkImageUsageFlags UidFrameBuilder::uidMapUsage() const noexcept
  {
    return _uidMapUsage;
  }
}