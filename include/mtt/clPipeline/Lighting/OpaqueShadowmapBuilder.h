#pragma once

#include <glm/mat4x4.hpp>

#include <mtt/clPipeline/RenderPass/OpaqueShadowmapPass.h>
#include <mtt/render/SceneRenderer/AbstractFrame.h>
#include <mtt/render/SceneRenderer/AbstractFrameBuilder.h>
#include <mtt/render/Ref.h>

namespace mtt
{
  class Image;

  namespace clPipeline
  {
    class OpaqueShadowmapBuilder : public AbstractFrameBuilder
    {
    public:
      class FramePlan : public AbstractFramePlan
      {
      protected:
        friend class OpaqueShadowmapBuilder;
        FramePlan(AbstractFrame& frame);

      public:
        FramePlan(const FramePlan&) = delete;
        FramePlan& operator = (const FramePlan&) = delete;
        virtual ~FramePlan() noexcept = default;

        glm::mat4 viewProjectionMatrix;   /// This value is setted externally
                                          /// and used as a tag to find
                                          /// duplicate frame queries

      private:
        DrawBin _renderBin;
      };

    public:
      OpaqueShadowmapBuilder( VkFormat shadowmapFormat,
                              VkImageLayout shadowmapLayout,
                              LogicalDevice& device);
      OpaqueShadowmapBuilder(const OpaqueShadowmapBuilder&) = delete;
      OpaqueShadowmapBuilder& operator = (
                                        const OpaqueShadowmapBuilder&) = delete;
      virtual ~OpaqueShadowmapBuilder() noexcept = default;

      inline VkFormat shadowmapFormat() const noexcept;
      inline VkImageLayout shadowmapLayout() const noexcept;

      inline LogicalDevice& device() const noexcept;

      /// Target should be VK_IMAGE_VIEW_TYPE_2D with shadowmapFormat and
      /// shadowmapLayout from OpaqueShadowmapBuilder's constructor and
      /// VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT usage flag
      std::unique_ptr<AbstractFrame> createFrame(ImageView& target);

      /// OpaqueShadowmapBuilder::FramePlan will be created
      virtual std::unique_ptr<AbstractFramePlan> createFramePlan(
                                          AbstractFrame& frame) const override;

      virtual void scheduleRender(AbstractFramePlan& plan,
                                  CommandProducer& drawProducer) override;
    private:
      VkFormat _shadowmapFormat;
      VkImageLayout _shadowmapLayout;
      Ref<OpaqueShadowmapPass> _renderPass;
      LogicalDevice& _device;
    };

    inline VkFormat OpaqueShadowmapBuilder::shadowmapFormat() const noexcept
    {
      return _shadowmapFormat;
    }

    inline VkImageLayout
                        OpaqueShadowmapBuilder::shadowmapLayout() const noexcept
    {
      return _shadowmapLayout;
    }

    inline LogicalDevice& OpaqueShadowmapBuilder::device() const noexcept
    {
      return _device;
    }
  }
}