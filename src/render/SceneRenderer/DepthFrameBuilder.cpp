#include <mtt/render/SceneRenderer/DepthFrameBuilder.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

class DepthFrame : public OneTargetFrameBuilder::Frame
{
public:
  Ref<FrameBuffer> frameBuffer;
  Ref<Image> target;
  Ref<ImageView> depthMap;

  DepthFrame(glm::u16vec2 extent, DepthFrameBuilder& renderer) :
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

class DepthFramePlan : public AbstractFramePlan
{
public:
  DepthFramePlan(AbstractFrame& theFrame) :
    AbstractFramePlan(theFrame),
    _depthMapBin(memoryPool())
  {
    registerBin(_depthMapBin, baseStages::depthmapStage);
  }

  DepthFramePlan(const DepthFramePlan&) = delete;
  DepthFramePlan& operator = (const DepthFramePlan&) = delete;
  virtual ~DepthFramePlan() noexcept = default;

private:
  DrawBin _depthMapBin;
};

DepthFrameBuilder::DepthFrameBuilder( VkFormat depthMapFormat,
                                      VkImageLayout depthMapLayout,
                                      VkImageUsageFlags depthmapUsage,
                                      LogicalDevice& device) :
  OneTargetFrameBuilder(baseFrameTypes::depthmapFrameType),
  _renderPass(new DepthMapRenderPass( depthMapFormat,
                                      depthMapLayout,
                                      device)),
  _device(device),
  _depthmapUsage(depthmapUsage)
{
  registerStagePass(*_renderPass);
}

std::unique_ptr<DepthFrame::Frame> DepthFrameBuilder::createFrame(
                                                      const glm::uvec2& extent)
{
  std::unique_ptr<DepthFrame> newFrame(new DepthFrame(extent , *this));

  newFrame->target = new Image( VK_IMAGE_TYPE_2D,
                                _renderPass->depthMapLayout(),
                                VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT |
                                  _depthmapUsage,
                                0,
                                depthMapFormat(),
                                glm::uvec3(extent, 1),
                                VK_SAMPLE_COUNT_1_BIT,
                                1,
                                1,
                                VK_IMAGE_ASPECT_DEPTH_BIT,
                                _device);

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

  newFrame->depthMap = new ImageView( *newFrame->target,
                                      VK_IMAGE_VIEW_TYPE_2D,
                                      colorMapping,
                                      depthSubresourceRange);

  newFrame->frameBuffer = _renderPass->createFrameBuffer(*newFrame->depthMap);
  return newFrame;
}

std::unique_ptr<AbstractFramePlan> DepthFrameBuilder::createFramePlan(
                                                    AbstractFrame& frame) const
{
  if(&frame.builder() != this) Abort("DepthFrameBuilder::createFramePlan: this frame was created by another builder.");
  return std::make_unique<DepthFramePlan>(frame);
}

void DepthFrameBuilder::scheduleRender( AbstractFramePlan& plan,
                                        CommandProducer& drawProducer)
{
  AbstractFrame& target = plan.frame();
  if(&target.builder() != this) Abort("DepthFrameBuilder::scheduleRender: the frame was not created in this renderer.");
  DepthFrame& frame = static_cast<DepthFrame&>(target);
  DrawContext drawContext{ drawProducer, frame.frameBuffer.get() };
  _renderPass->scheduleRender(plan, drawContext);
}
