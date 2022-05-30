#pragma once

#include <memory>

#include <glm/mat4x4.hpp>

#include <mtt/clPipeline/RenderPass/OpaqueShadowmapPass.h>
#include <mtt/clPipeline/RenderPass/TransparentShadowmapPass.h>
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

        glm::mat4 viewProjectionMatrix;   /// This value is setted externally
                                          /// and used as a tag to find
                                          /// duplicate frame queries

      private:
        DrawBin _opaqueShadowmapBin;
        DrawBin _transparentShadowmapBin;
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
      Ref<OpaqueShadowmapPass> _opaquePass;
      Ref<TransparentShadowmapPass> _transparentPass;
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