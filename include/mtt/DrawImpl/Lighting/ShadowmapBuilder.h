#pragma once

#include <memory>

#include <mtt/DrawImpl/Lighting/ShadowmapBlurFilter.h>
#include <mtt/DrawImpl/RenderPass/ShadowmapRenderPass.h>
#include <mtt/Render/SceneRenderer/AbstractFrame.h>
#include <mtt/Render/SceneRenderer/OneTargetFrameBuilder.h>
#include <mtt/Render/Ref.h>

namespace mtt
{
  class Image;

  /// Target image in ShadowmapRenderPass::shadowmapFormat and
  /// VK_IMAGE_ASPECT_COLOR_BIT aspect.
  /// R component is distance, G component is square of distance;
  class ShadowmapBuilder : public OneTargetFrameBuilder
  {
  public:
    class ShadowMapFramePlan : public AbstractFramePlan
    {
    protected:
      friend class ShadowmapBuilder;
      ShadowMapFramePlan(AbstractFrame& frame);

    public:
      ShadowMapFramePlan(const ShadowMapFramePlan&) = delete;
      ShadowMapFramePlan& operator = (const ShadowMapFramePlan&) = delete;
      virtual ~ShadowMapFramePlan() noexcept = default;

    public:
      float blurLevel;

    private:
      DrawBin _shadowmapBin;
    };

  public:
    ShadowmapBuilder( VkFormat shadowmapFormat,
                      VkImageLayout shadowmapLayout,
                      VkImageUsageFlags shadowmapUsage,
                      LogicalDevice& device);
    ShadowmapBuilder(const ShadowmapBuilder&) = delete;
    ShadowmapBuilder& operator = (const ShadowmapBuilder&) = delete;
    virtual ~ShadowmapBuilder() noexcept = default;

    inline VkFormat shadowmapFormat() const noexcept;
    inline VkImageLayout shadowmapLayout() const noexcept;
    inline VkImageUsageFlags shadowmapUsage() const noexcept;

    virtual std::unique_ptr<Frame> createFrame(
                                            const glm::uvec2& extent) override;

    /// ShadowMapFramePlan will be created
    virtual std::unique_ptr<AbstractFramePlan> createFramePlan(
                                          AbstractFrame& frame) const override;
    /// ShadowMapFramePlan will be created
    std::unique_ptr<ShadowMapFramePlan> createFramePlan(AbstractFrame& frame,
                                                        float blurLevel) const;

    virtual void scheduleRender(AbstractFramePlan& plan,
                                CommandProducer& drawProducer) override;
  private:
    VkFormat _shadowmapFormat;
    VkImageLayout _shadowmapLayout;
    VkImageUsageFlags _shadowmapUsage;
    Ref<ShadowmapRenderPass> _renderPass;
    Ref<ShadowmapBlurFilter> _blurFilter;
    LogicalDevice& _device;
  };

  inline VkFormat ShadowmapBuilder::shadowmapFormat() const noexcept
  {
    return _shadowmapFormat;
  }

  inline VkImageLayout ShadowmapBuilder::shadowmapLayout() const noexcept
  {
    return _shadowmapLayout;
  }

  inline VkImageUsageFlags ShadowmapBuilder::shadowmapUsage() const noexcept
  {
    return _shadowmapUsage;
  }
}