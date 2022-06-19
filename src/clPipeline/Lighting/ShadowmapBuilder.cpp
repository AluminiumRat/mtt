#include <mtt/clPipeline/Lighting/ShadowmapBuilder.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/utilities/Abort.h>
#include <mtt/utilities/static_cast_unique_ptr.h>

using namespace mtt;
using namespace mtt::clPipeline;

static constexpr VkFormat depthBufferFormat = VK_FORMAT_D32_SFLOAT;
static constexpr VkImageLayout depthBufferLayout =
                              VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

class ShadowmapBuilderFrame : public AbstractFrame
{
public:
  Ref<FrameBuffer> opaquePassFrameBuffer;
  Ref<FrameBuffer> transparentPassFrameBuffer;

  Ref<Image> target;

  ShadowmapBuilderFrame(glm::u16vec2 extent, ShadowmapBuilder& renderer) :
    AbstractFrame(extent, renderer)
  {
  }
};

ShadowmapBuilder::ShadowMapFramePlan::ShadowMapFramePlan(AbstractFrame& frame) :
  AbstractFramePlan(frame),
  viewProjectionMatrix(1),
  _opaqueShadowmapBin(memoryPool()),
  _transparentShadowmapBin(memoryPool())
{
  registerBin(_opaqueShadowmapBin, opaqueShadowmapStage);
  registerBin(_transparentShadowmapBin, transparentShadowmapStage);
}

ShadowmapBuilder::ShadowmapBuilder( VkFormat shadowmapFormat,
                                    VkImageLayout shadowmapLayout,
                                    LogicalDevice& device) :
  AbstractFrameBuilder(shadowmapFrameType),
  _shadowmapFormat(shadowmapFormat),
  _shadowmapLayout(shadowmapLayout),
  _opaquePass(new OpaqueShadowmapPass(_shadowmapFormat,
                                      _shadowmapLayout,
                                      depthBufferFormat,
                                      depthBufferLayout,
                                      device)),
  _transparentPass(new TransparentShadowmapPass(_shadowmapFormat,
                                                _shadowmapLayout,
                                                device)),
  _device(device)
{
  registerStagePass(*_opaquePass);
  registerStagePass(*_transparentPass);
}

std::unique_ptr<AbstractFrame> ShadowmapBuilder::createFrame(ImageView& target)
{
  if(target.viewType() != VK_IMAGE_VIEW_TYPE_2D) Abort("ShadowmapBuilder::createFrame: target type is not VK_IMAGE_VIEW_TYPE_2D");
  if(target.image().format() != _shadowmapFormat) Abort("ShadowmapBuilder::createFrame: wrong target format");
  if (target.image().primaryLayout() != _shadowmapLayout) Abort("ShadowmapBuilder::createFrame: wrong target layout");

  glm::uvec2 extent = target.extent();

  std::unique_ptr<ShadowmapBuilderFrame> newFrame(
                                    new ShadowmapBuilderFrame(extent , *this));

  Ref<Image> depthBuffer(new Image( VK_IMAGE_TYPE_2D,
                                    depthBufferLayout,
                                    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                                    0,
                                    depthBufferFormat,
                                    glm::uvec3(extent,1),
                                    VK_SAMPLE_COUNT_1_BIT,
                                    1,
                                    1,
                                    VK_IMAGE_ASPECT_DEPTH_BIT,
                                    _device));

  VkComponentMapping colorMapping{};
  colorMapping.r = VK_COMPONENT_SWIZZLE_IDENTITY;
  colorMapping.g = VK_COMPONENT_SWIZZLE_IDENTITY;
  colorMapping.b = VK_COMPONENT_SWIZZLE_IDENTITY;
  colorMapping.a = VK_COMPONENT_SWIZZLE_IDENTITY;

  VkImageSubresourceRange depthSubresourceRange{};
  depthSubresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
  depthSubresourceRange.baseMipLevel = 0;
  depthSubresourceRange.levelCount = 1;
  depthSubresourceRange.baseArrayLayer = 0;
  depthSubresourceRange.layerCount = 1;

  Ref<ImageView>depthBufferView(new ImageView(*depthBuffer,
                                              VK_IMAGE_VIEW_TYPE_2D,
                                              colorMapping,
                                              depthSubresourceRange));

  newFrame->opaquePassFrameBuffer = _opaquePass->createFrameBuffer(
                                                              target,
                                                              *depthBufferView);

  newFrame->transparentPassFrameBuffer =
                                    _transparentPass->createFrameBuffer(target);

  return newFrame;
}

std::unique_ptr<AbstractFramePlan> ShadowmapBuilder::createFramePlan(
                                                    AbstractFrame& frame) const
{
  if(&frame.builder() != this) Abort("ShadowmapBuilder::createFramePlan: the frame was created in another builder.");
  return std::unique_ptr<AbstractFramePlan>(new ShadowMapFramePlan(frame));
}

void ShadowmapBuilder::scheduleRender(AbstractFramePlan& plan,
                                      CommandProducer& drawProducer)
{
  if(&plan.frameBuilder() != this) Abort("ShadowmapBuilder::scheduleRender: the frame was not created in this renderer.");

  ShadowMapFramePlan& shadowPlan = static_cast<ShadowMapFramePlan&>(plan);
  ShadowmapBuilderFrame& frame =
                              static_cast<ShadowmapBuilderFrame&>(plan.frame());

  DrawContext drawContext{drawProducer, frame.opaquePassFrameBuffer.get()};
  _opaquePass->scheduleRender(plan, drawContext);

  drawContext.frameBuffer = frame.transparentPassFrameBuffer.get();
  _transparentPass->scheduleRender(plan, drawContext);
}
