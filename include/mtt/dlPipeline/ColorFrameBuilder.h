#pragma once

#include <memory>

#include <mtt/dlPipeline/RenderPass/AdditionalColorPass.h>
#include <mtt/dlPipeline/RenderPass/AmbientWeightPass.h>
#include <mtt/dlPipeline/RenderPass/BackgroundPass.h>
#include <mtt/dlPipeline/RenderPass/EmissionPass.h>
#include <mtt/dlPipeline/RenderPass/GBufferPass.h>
#include <mtt/dlPipeline/RenderPass/LightingPass.h>
#include <mtt/dlPipeline/RenderPass/PeelingGPass.h>
#include <mtt/dlPipeline/TransparencyPipeline.h>
#include <mtt/render/SceneRenderer/AbstractFrame.h>
#include <mtt/render/SceneRenderer/AbstractColorFrameBuilder.h>
#include <mtt/render/Ref.h>

namespace mtt
{
  class Image;

  namespace dlPipeline
  {
    class ColorFrameBuilder : public AbstractColorFrameBuilder
    {
    public:
      static constexpr VkFormat depthMapFormat = VK_FORMAT_D32_SFLOAT;
      static constexpr VkFormat albedoMapFormat = VK_FORMAT_B8G8R8A8_SRGB;
      static constexpr VkFormat specularMapFormat = VK_FORMAT_B8G8R8A8_SRGB;
      static constexpr VkFormat normalMapFormat = VK_FORMAT_R16G16B16A16_SNORM;
      static constexpr VkFormat ambientWeightFormat = VK_FORMAT_R32_SFLOAT;

    public:
      ColorFrameBuilder(VkFormat colorBufferFormat, LogicalDevice& device);
      ColorFrameBuilder(const ColorFrameBuilder&) = delete;
      ColorFrameBuilder& operator = (const ColorFrameBuilder&) = delete;
      virtual ~ColorFrameBuilder() noexcept = default;

      virtual std::unique_ptr<AbstractFrame> createFrame(
                                                        Image& target) override;

      /// The frame should be created by this frame renderer
      virtual std::unique_ptr<AbstractFramePlan> createFramePlan(
                                          AbstractFrame& frame) const override;

      virtual void scheduleRender(AbstractFramePlan& plan,
                                  CommandProducer& drawProducer) override;

    private:
      Ref<GBufferPass> _gBufferPass;
      Ref<AmbientWeightPass> _ambientWeightPass;
      Ref<LightingPass> _lightingPass;
      Ref<EmissionPass> _emissionPass;
      Ref<BackgroundPass> _backgroundPass;
      Ref<PeelingGPass> _peelingGPass;
      Ref<AmbientWeightPass> _peelingAmbientWeightPass;
      Ref<LightingPass> _peelingLightingPass;
      Ref<BackgroundPass> _peelingBackgroundPass;
      Ref<AdditionalColorPass> _instrumentPass;
      Ref<AdditionalColorPass> _selectionPass;

      TransparencyPipeline _transparencyPipeline;

      LogicalDevice& _device;
    };
  }
}