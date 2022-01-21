#pragma once

#include <memory>

#include <mtt/render/RenderPass/DepthMapRenderPass.h>
#include <mtt/render/SceneRenderer/AbstractFrame.h>
#include <mtt/render/SceneRenderer/OneTargetFrameBuilder.h>
#include <mtt/render/Ref.h>

namespace mtt
{
  class Image;

  /// Target image in VK_IMAGE_ASPECT_DEPTH_BIT aspect.
  class DepthFrameBuilder : public OneTargetFrameBuilder
  {
  public:
    DepthFrameBuilder(VkFormat depthMapFormat,
                      VkImageLayout depthMapLayout,
                      VkImageUsageFlags depthmapUsage,
                      LogicalDevice& device);
    DepthFrameBuilder(const DepthFrameBuilder&) = delete;
    DepthFrameBuilder& operator = (const DepthFrameBuilder&) = delete;
    virtual ~DepthFrameBuilder() noexcept = default;

    virtual std::unique_ptr<Frame> createFrame(
                                            const glm::uvec2& extent) override;
    /// The frame should be created by this frame renderer
    virtual std::unique_ptr<AbstractFramePlan> createFramePlan(
                                          AbstractFrame& frame) const override;

    virtual void scheduleRender(AbstractFramePlan& plan,
                                CommandProducer& drawProducer) override;

    inline VkFormat depthMapFormat() const noexcept;
    inline VkImageLayout depthMapLayout() const noexcept;
    inline VkImageUsageFlags depthmapUsage() const noexcept;

  private:
    VkImageUsageFlags _depthmapUsage;
    Ref<DepthMapRenderPass> _renderPass;
    LogicalDevice& _device;
  };

  inline VkFormat DepthFrameBuilder::depthMapFormat() const noexcept
  {
    return _renderPass->depthMapFormat();
  }

  inline VkImageLayout DepthFrameBuilder::depthMapLayout() const noexcept
  {
    return _renderPass->depthMapLayout();
  }

  inline VkImageUsageFlags DepthFrameBuilder::depthmapUsage() const noexcept
  {
    return _depthmapUsage;
  }
}