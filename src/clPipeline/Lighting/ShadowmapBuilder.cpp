#include <mtt/clPipeline/Lighting/ShadowmapBuilder.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/utilities/Abort.h>
#include <mtt/utilities/static_cast_unique_ptr.h>

using namespace mtt;
using namespace mtt::clPipeline;

static constexpr VkFormat depthBufferFormat = VK_FORMAT_D32_SFLOAT;
static constexpr VkImageLayout depthBufferLayout =
                              VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

class LinearDepthFrame : public OneTargetFrameBuilder::Frame
{
public:
  Ref<FrameBuffer> mainPassFrameBuffer;

  Ref<Image> target;
  Ref<ImageView> targetView;
  Ref<ImageView> depthBufferView;

  LinearDepthFrame(glm::u16vec2 extent, ShadowmapBuilder& renderer) :
    Frame(extent, renderer)
  {
  }

  virtual Image& targetImage() noexcept override
  {
    return *target;
  }

  virtual const Image& targetImage() const noexcept override
  {
    return *target;
  }
};

ShadowmapBuilder::ShadowMapFramePlan::ShadowMapFramePlan(AbstractFrame& frame) :
  AbstractFramePlan(frame),
  _shadowmapBin(memoryPool())
{
  registerBin(_shadowmapBin, shadowmapStage);
}

ShadowmapBuilder::ShadowmapBuilder( VkFormat shadowmapFormat,
                                    VkImageLayout shadowmapLayout,
                                    VkImageUsageFlags shadowmapUsage,
                                    LogicalDevice& device) :
  OneTargetFrameBuilder(shadowmapFrameType),
  _shadowmapFormat(shadowmapFormat),
  _shadowmapLayout(shadowmapLayout),
  _shadowmapUsage(shadowmapUsage),
  _renderPass(new ShadowmapPass(_shadowmapFormat,
                                _shadowmapLayout,
                                depthBufferFormat,
                                depthBufferLayout,
                                device)),
  _device(device)
{
  registerStagePass(*_renderPass);
}

std::unique_ptr<ShadowmapBuilder::Frame>
                        ShadowmapBuilder::createFrame(const glm::uvec2& extent)
{
  std::unique_ptr<LinearDepthFrame> newFrame(
                                          new LinearDepthFrame(extent , *this));

  newFrame->target = new Image( VK_IMAGE_TYPE_2D,
                                _shadowmapLayout,
                                VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
                                  VK_IMAGE_USAGE_SAMPLED_BIT |
                                  _shadowmapUsage,
                                0,
                                _shadowmapFormat,
                                glm::uvec3(extent, 1),
                                VK_SAMPLE_COUNT_1_BIT,
                                1,
                                1,
                                VK_IMAGE_ASPECT_COLOR_BIT,
                                _device);

  VkComponentMapping colorMapping{};
  colorMapping.r = VK_COMPONENT_SWIZZLE_IDENTITY;
  colorMapping.g = VK_COMPONENT_SWIZZLE_IDENTITY;
  colorMapping.b = VK_COMPONENT_SWIZZLE_IDENTITY;
  colorMapping.a = VK_COMPONENT_SWIZZLE_IDENTITY;

  VkImageSubresourceRange colorSubresourceRange{};
  colorSubresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  colorSubresourceRange.baseMipLevel = 0;
  colorSubresourceRange.levelCount = 1;
  colorSubresourceRange.baseArrayLayer = 0;
  colorSubresourceRange.layerCount = 1;

  Ref<ImageView> targetView(new ImageView(*newFrame->target,
                                          VK_IMAGE_VIEW_TYPE_2D,
                                          colorMapping,
                                          colorSubresourceRange));
  newFrame->targetView = std::move(targetView);

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

  VkImageSubresourceRange depthSubresourceRange{};
  depthSubresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
  depthSubresourceRange.baseMipLevel = 0;
  depthSubresourceRange.levelCount = 1;
  depthSubresourceRange.baseArrayLayer = 0;
  depthSubresourceRange.layerCount = 1;

  newFrame->depthBufferView = new ImageView(*depthBuffer,
                                            VK_IMAGE_VIEW_TYPE_2D,
                                            colorMapping,
                                            depthSubresourceRange);

  newFrame->mainPassFrameBuffer = _renderPass->createFrameBuffer(
                                                    *newFrame->targetView,
                                                    *newFrame->depthBufferView);
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
  LinearDepthFrame& frame = static_cast<LinearDepthFrame&>(plan.frame());
  
  DrawContext drawContext{drawProducer, frame.mainPassFrameBuffer.get()};
  _renderPass->scheduleRender(plan, drawContext);
}
