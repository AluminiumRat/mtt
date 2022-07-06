#include <mtt/clPipeline/Lighting/TransparentShadowmapBuilder.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;
using namespace mtt::clPipeline;

class TransparentShadowmapBuilder::FramePlan : public AbstractFramePlan
{
protected:
  friend class TransparentShadowmapBuilder;
  FramePlan(AbstractFrame& frame);

public:
  FramePlan(const FramePlan&) = delete;
  FramePlan& operator = (const FramePlan&) = delete;
  virtual ~FramePlan() noexcept = default;

private:
  DrawBin _renderBin;
};

class TransparentShadowmapBuilder::Frame : public AbstractFrame
{
public:
  Ref<FrameBuffer> frameBuffer;

  Ref<Image> target;

  Frame(glm::u16vec2 extent, TransparentShadowmapBuilder& renderer) :
    AbstractFrame(extent, renderer)
  {
  }
};

TransparentShadowmapBuilder::FramePlan::FramePlan(AbstractFrame& frame) :
  AbstractFramePlan(frame),
  _renderBin(memoryPool())
{
  registerBin(_renderBin, shadowmapStage);
}

TransparentShadowmapBuilder::TransparentShadowmapBuilder(
                                                  VkFormat shadowmapFormat,
                                                  VkImageLayout shadowmapLayout,
                                                  LogicalDevice& device) :
  AbstractFrameBuilder(transparentShadowmapFrameType),
  _shadowmapFormat(shadowmapFormat),
  _shadowmapLayout(shadowmapLayout),
  _renderPass(new TransparentShadowmapPass( _shadowmapFormat,
                                            _shadowmapLayout,
                                            device)),
  _device(device)
{
  registerStagePass(*_renderPass);
}

std::unique_ptr<AbstractFrame> TransparentShadowmapBuilder::createFrame(
                                                              ImageView& target)
{
  if(target.viewType() != VK_IMAGE_VIEW_TYPE_2D) Abort("TransparentShadowmapBuilder::createFrame: target type is not VK_IMAGE_VIEW_TYPE_2D");
  if(target.image().format() != _shadowmapFormat) Abort("TransparentShadowmapBuilder::createFrame: wrong target format");
  if (target.image().primaryLayout() != _shadowmapLayout) Abort("TransparentShadowmapBuilder::createFrame: wrong target layout");

  std::unique_ptr<Frame> newFrame(new Frame(target.extent(), *this));
  newFrame->frameBuffer = _renderPass->createFrameBuffer(target);

  return newFrame;
}

std::unique_ptr<AbstractFramePlan>
        TransparentShadowmapBuilder::createFramePlan(AbstractFrame& frame) const
{
  if(&frame.builder() != this) Abort("TransparentShadowmapBuilder::createFramePlan: the frame was created in another builder.");
  return std::unique_ptr<AbstractFramePlan>(new FramePlan(frame));
}

void TransparentShadowmapBuilder::scheduleRender( AbstractFramePlan& plan,
                                                  CommandProducer& drawProducer)
{
  if(&plan.frameBuilder() != this) Abort("TransparentShadowmapBuilder::scheduleRender: the frame was not created in this renderer.");

  FramePlan& shadowPlan = static_cast<FramePlan&>(plan);
  Frame& frame = static_cast<Frame&>(plan.frame());

  DrawContext drawContext{drawProducer, frame.frameBuffer.get()};
  _renderPass->scheduleRender(plan, drawContext);
}
