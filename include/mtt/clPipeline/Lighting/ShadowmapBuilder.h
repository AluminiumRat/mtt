#pragma once

#include <memory>

#include <mtt/clPipeline/RenderPass/ShadowmapPass.h>
#include <mtt/render/SceneRenderer/AbstractFrame.h>
#include <mtt/render/SceneRenderer/OneTargetFrameBuilder.h>
#include <mtt/render/Ref.h>

namespace mtt
{
  class Image;

  namespace clPipeline
  {
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

      virtual void scheduleRender(AbstractFramePlan& plan,
                                  CommandProducer& drawProducer) override;
    private:
      VkFormat _shadowmapFormat;
      VkImageLayout _shadowmapLayout;
      VkImageUsageFlags _shadowmapUsage;
      Ref<ShadowmapPass> _renderPass;
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
}