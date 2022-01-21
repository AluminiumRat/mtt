#include <mtt/render/SceneRenderer/UidFrameBuilder.h>

using namespace mtt;

static constexpr VkFormat depthMapFormat = VK_FORMAT_D32_SFLOAT;

class UidFrame : public OneTargetFrameBuilder::Frame
{
public:
  Ref<FrameBuffer> uidFrameBuffer;

  Ref<Image> target;
  Ref<ImageView> targetView;
  Ref<ImageView> depthMap;

  UidFrame(glm::u16vec2 extent, UidFrameBuilder& renderer) :
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

class UidFramePlan : public AbstractFramePlan
{
public:
  UidFramePlan(AbstractFrame& theFrame) :
    AbstractFramePlan(theFrame),
    _uidBin(memoryPool())
  {
    registerBin(_uidBin, baseStages::uidStage);
  }

  UidFramePlan(const UidFramePlan&) = delete;
  UidFramePlan& operator = (const UidFramePlan&) = delete;
  virtual ~UidFramePlan() noexcept = default;

private:
  DrawBin _uidBin;
};

UidFrameBuilder::UidFrameBuilder( VkFormat uidMapFormat,
                                  VkImageLayout uidMapLayout,
                                  VkImageUsageFlags uidMapUsage,
                                  LogicalDevice& device) :
  OneTargetFrameBuilder(baseFrameTypes::uidFrameType),
  _uidMapUsage(uidMapUsage),
  _renderPass(new UIDRenderPass(
                              uidMapFormat,
                              uidMapLayout,
                              depthMapFormat,
                              VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                              device)),
  _device(device)
{
  registerStagePass(*_renderPass);
}

std::unique_ptr<UidFrameBuilder::Frame> UidFrameBuilder::createFrame(
                                                      const glm::uvec2& extent)
{
  std::unique_ptr<UidFrame> newFrame(new UidFrame(extent , *this));

  newFrame->target = new Image( VK_IMAGE_TYPE_2D,
                                uidMapLayout(),
                                _uidMapUsage |
                                  VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                                0,
                                uidMapFormat(),
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

  newFrame->targetView = new ImageView( *newFrame->target,
                                        VK_IMAGE_VIEW_TYPE_2D,
                                        colorMapping,
                                        colorSubresourceRange);

  Ref<Image> depthBuffer(new Image(
                              VK_IMAGE_TYPE_2D,
                              VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                              VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                              0,
                              depthMapFormat,
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

  newFrame->depthMap = new ImageView( *depthBuffer,
                                      VK_IMAGE_VIEW_TYPE_2D,
                                      colorMapping,
                                      depthSubresourceRange);

  newFrame->uidFrameBuffer = _renderPass->createFrameBuffer(
                                                        *newFrame->targetView,
                                                        *newFrame->depthMap);
  return newFrame;
}

std::unique_ptr<AbstractFramePlan> UidFrameBuilder::createFramePlan(
                                                    AbstractFrame& frame) const
{
  if(&frame.builder() != this) Abort("UidFrameBuilder::createFramePlan: this frame was created by another builder.");
  return std::make_unique<UidFramePlan>(frame);
}

void UidFrameBuilder::scheduleRender( AbstractFramePlan& plan,
                                      CommandProducer& drawProducer)
{
  AbstractFrame& target = plan.frame();
  if(&target.builder() != this) Abort("UidFrameBuilder::scheduleRender: the frame was not created in this renderer.");
  UidFrame& frame = static_cast<UidFrame&>(target);
  DrawContext drawContext{ drawProducer, frame.uidFrameBuffer.get()};
  _renderPass->scheduleRender(plan, drawContext);
}
