#pragma once

#include <mtt/clPipeline/RenderPass/TransparentShadowmapPass.h>
#include <mtt/render/SceneRenderer/AbstractFrame.h>
#include <mtt/render/SceneRenderer/AbstractFrameBuilder.h>
#include <mtt/render/Ref.h>

namespace mtt
{
  class Image;

  namespace clPipeline
  {
    class TransparentShadowmapBuilder : public AbstractFrameBuilder
    {
    public:
      TransparentShadowmapBuilder(VkFormat shadowmapFormat,
                                  VkImageLayout shadowmapLayout,
                                  LogicalDevice& device);
      TransparentShadowmapBuilder(const TransparentShadowmapBuilder&) = delete;
      TransparentShadowmapBuilder& operator = (
                                  const TransparentShadowmapBuilder&) = delete;
      virtual ~TransparentShadowmapBuilder() noexcept = default;

      inline VkFormat shadowmapFormat() const noexcept;
      inline VkImageLayout shadowmapLayout() const noexcept;

      inline LogicalDevice& device() const noexcept;

      /// Target should be VK_IMAGE_VIEW_TYPE_2D with shadowmapFormat and
      /// shadowmapLayout from TransparentShadowmapBuilder's constructor and
      /// VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT usage flag
      std::unique_ptr<AbstractFrame> createFrame(ImageView& target);

      virtual std::unique_ptr<AbstractFramePlan> createFramePlan(
                                          AbstractFrame& frame) const override;

      virtual void scheduleRender(AbstractFramePlan& plan,
                                  CommandProducer& drawProducer) override;

    private:
      class FramePlan;
      class Frame;

    private:
      VkFormat _shadowmapFormat;
      VkImageLayout _shadowmapLayout;
      Ref<TransparentShadowmapPass> _renderPass;
      LogicalDevice& _device;
    };

    inline VkFormat TransparentShadowmapBuilder::shadowmapFormat() const noexcept
    {
      return _shadowmapFormat;
    }

    inline VkImageLayout TransparentShadowmapBuilder::shadowmapLayout() const noexcept
    {
      return _shadowmapLayout;
    }

    inline LogicalDevice& TransparentShadowmapBuilder::device() const noexcept
    {
      return _device;
    }
  }
}