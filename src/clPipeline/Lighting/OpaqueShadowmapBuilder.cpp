#include <mtt/clPipeline/Lighting/OpaqueShadowmapBuilder.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;
using namespace mtt::clPipeline;

static constexpr VkFormat depthBufferFormat = VK_FORMAT_D32_SFLOAT;
static constexpr VkImageLayout depthBufferLayout =
                              VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

class OpaqueShadowmapBuilderFrame : public AbstractFrame
{
public:
  Ref<FrameBuffer> frameBuffer;
  Ref<Image> target;

  OpaqueShadowmapBuilderFrame(glm::u16vec2 extent, OpaqueShadowmapBuilder& renderer) :
    AbstractFrame(extent, renderer)
  {
  }
};

OpaqueShadowmapBuilder::FramePlan::FramePlan(AbstractFrame& frame) :
  AbstractFramePlan(frame),
  viewProjectionMatrix(1),
  _renderBin(memoryPool())
{
  registerBin(_renderBin, shadowmapStage);
}

OpaqueShadowmapBuilder::OpaqueShadowmapBuilder( VkFormat shadowmapFormat,
                                    VkImageLayout shadowmapLayout,
                                    LogicalDevice& device) :
  AbstractFrameBuilder(opaqueShadowmapFrameType),
  _shadowmapFormat(shadowmapFormat),
  _shadowmapLayout(shadowmapLayout),
  _renderPass(new OpaqueShadowmapPass(_shadowmapFormat,
                                      _shadowmapLayout,
                                      depthBufferFormat,
                                      depthBufferLayout,
                                      device)),
  _device(device)
{
  registerStagePass(*_renderPass);
}

std::unique_ptr<AbstractFrame>
                          OpaqueShadowmapBuilder::createFrame(ImageView& target)
{
  if(target.viewType() != VK_IMAGE_VIEW_TYPE_2D) Abort("OpaqueShadowmapBuilder::createFrame: target type is not VK_IMAGE_VIEW_TYPE_2D");
  if(target.image().format() != _shadowmapFormat) Abort("OpaqueShadowmapBuilder::createFrame: wrong target format");
  if (target.image().primaryLayout() != _shadowmapLayout) Abort("OpaqueShadowmapBuilder::createFrame: wrong target layout");

  glm::uvec2 extent = target.extent();

  std::unique_ptr<OpaqueShadowmapBuilderFrame> newFrame(
                                    new OpaqueShadowmapBuilderFrame(extent , *this));

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

  newFrame->frameBuffer = _renderPass->createFrameBuffer( target,
                                                          *depthBufferView);
  return newFrame;
}

std::unique_ptr<AbstractFramePlan> OpaqueShadowmapBuilder::createFramePlan(
                                                    AbstractFrame& frame) const
{
  if(&frame.builder() != this) Abort("OpaqueShadowmapBuilder::createFramePlan: the frame was created in another builder.");
  return std::unique_ptr<AbstractFramePlan>(new FramePlan(frame));
}

void OpaqueShadowmapBuilder::scheduleRender(AbstractFramePlan& plan,
                                            CommandProducer& drawProducer)
{
  if(&plan.frameBuilder() != this) Abort("OpaqueShadowmapBuilder::scheduleRender: the frame was not created in this renderer.");

  FramePlan& framePlan = static_cast<FramePlan&>(plan);
  OpaqueShadowmapBuilderFrame& frame =
                        static_cast<OpaqueShadowmapBuilderFrame&>(plan.frame());

  DrawContext drawContext{drawProducer, frame.frameBuffer.get()};
  _renderPass->scheduleRender(plan, drawContext);
}
