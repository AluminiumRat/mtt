#include <mtt/clPipeline/ColorFrameBuilder.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;
using namespace mtt::clPipeline;

static constexpr VkImageLayout commonDepthBufferLayout =
                              VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

static constexpr VkImageUsageFlags depthImageUsage =
                                  VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT |
                                  VK_IMAGE_USAGE_SAMPLED_BIT |
                                  VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT |
                                  VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

static constexpr VkImageLayout flDepthSamplerLayout =
                                      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

static constexpr VkImageUsageFlags flDepthSamplerUsage =
                                                VK_IMAGE_USAGE_SAMPLED_BIT |
                                                VK_IMAGE_USAGE_TRANSFER_DST_BIT;

static constexpr VkImageLayout normalMapLayout =
                                      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

static constexpr VkImageUsageFlags normalImageUsage =
                                          VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
                                          VK_IMAGE_USAGE_SAMPLED_BIT |
                                          VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;

static constexpr VkImageLayout albedoMapLayout =
                                      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

static constexpr VkImageUsageFlags albedoImageUsage =
                                          VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
                                          VK_IMAGE_USAGE_SAMPLED_BIT |
                                          VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;

static constexpr VkImageLayout specularMapLayout =
                                      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

static constexpr VkImageUsageFlags specularImageUsage =
                                          VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
                                          VK_IMAGE_USAGE_SAMPLED_BIT |
                                          VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;

static constexpr VkImageLayout ambientWeightMapLayout =
                                      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

static constexpr VkImageUsageFlags ambientWeightImageUsage =
                                          VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
                                          VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT |
                                          VK_IMAGE_USAGE_SAMPLED_BIT;

static constexpr VkImageLayout commonColorBufferLayout =
                                      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

class ColorFrame : public AbstractFrame
{
public:
  Ref<FrameBuffer> geometryPassFrameBuffer;
  Ref<FrameBuffer> ambietWeightPassFrameBuffer;
  Ref<FrameBuffer> lightingPassFrameBuffer;
  Ref<FrameBuffer> emissionPassFrameBuffer;
  Ref<FrameBuffer> backgroundPassFrameBuffer;
  Ref<FrameBuffer> forwardLightPassFrameBuffer;
  Ref<FrameBuffer> selectionPassFrameBuffer;

  Image* colorBuffer;
  Image* target;
  Image* commonDepthBuffer;
  Image* flDepthSamplerImage;

  ColorFrame(const glm::uvec2& extent, ColorFrameBuilder& renderer) :
    AbstractFrame(extent, renderer)
  {
  }
};

class ColorFramePlan : public AbstractFramePlan
{
public:
  ColorFramePlan(AbstractFrame& theFrame) :
    AbstractFramePlan(theFrame),
    modificatorsPrepareBin(memoryPool()),
    gbufferBin(memoryPool()),
    ambientWeightBin(memoryPool()),
    lightingBin(memoryPool()),
    emissionBin(memoryPool()),
    backgroundBin(memoryPool()),
    forwardLightBin(memoryPool()),
    selectionBin(memoryPool())
  {
    registerBin(modificatorsPrepareBin, modificatorsPrepareStage);
    registerBin(gbufferBin, gBufferStage);
    registerBin(ambientWeightBin, ambientWeightStage);
    registerBin(lightingBin, lightingStage);
    registerBin(emissionBin, emissionStage);
    registerBin(backgroundBin, backgroundStage);
    registerBin(forwardLightBin, forwardLightStage);
    registerBin(selectionBin, selectionStage);
  }

  ColorFramePlan(const ColorFramePlan&) = delete;
  ColorFramePlan& operator = (const ColorFramePlan&) = delete;
  virtual ~ColorFramePlan() noexcept = default;

private:
  DrawBin modificatorsPrepareBin;
  DrawBin gbufferBin;
  DrawBin ambientWeightBin;
  DrawBin lightingBin;
  DrawBin emissionBin;
  DrawBin backgroundBin;
  DrawBin forwardLightBin;
  DrawBin selectionBin;
};

ColorFrameBuilder::ColorFrameBuilder( VkFormat colorBufferFormat,
                                      LogicalDevice& device) :
  AbstractColorFrameBuilder(),
  _gBufferPass(new GBufferPass( depthMapFormat,
                                commonDepthBufferLayout,
                                normalMapFormat,
                                normalMapLayout,
                                albedoMapFormat,
                                albedoMapLayout,
                                specularMapFormat,
                                specularMapLayout,
                                device)),
  _ambientWeightPass(new AmbientWeightPass( ambientWeightFormat,
                                            ambientWeightMapLayout,
                                            device)),
  _lightingPass(new LightingPass( VK_ATTACHMENT_LOAD_OP_CLEAR,
                                  colorBufferFormat,
                                  commonColorBufferLayout,
                                  device)),
  _emissionPass(new EmissionPass( VK_ATTACHMENT_LOAD_OP_LOAD,
                                  colorBufferFormat,
                                  commonColorBufferLayout,
                                  depthMapFormat,
                                  commonDepthBufferLayout,
                                  device)),
  _backgroundPass(new BackgroundPass( colorBufferFormat,
                                      commonColorBufferLayout,
                                      depthMapFormat,
                                      commonDepthBufferLayout,
                                      device)),
  _forwardLightPass(new ForwardLightPass( colorBufferFormat,
                                          commonColorBufferLayout,
                                          depthMapFormat,
                                          commonDepthBufferLayout,
                                          depthMapFormat,
                                          flDepthSamplerLayout,
                                          device)),
  _selectionPass(new SelectionPass( colorBufferFormat,
                                    commonColorBufferLayout,
                                    depthMapFormat,
                                    commonDepthBufferLayout,
                                    device)),
  _device(device)
{
  registerStagePass(*_gBufferPass);
  registerStagePass(*_ambientWeightPass);
  registerStagePass(*_lightingPass);
  registerStagePass(*_emissionPass);
  registerStagePass(*_backgroundPass);
  registerStagePass(*_forwardLightPass);
  registerStagePass(*_selectionPass);
}

std::unique_ptr<AbstractFrame> ColorFrameBuilder::createFrame(Image& target)
{
  if (&target.device() != &_device) Abort("ColorFrameBuilder::createFrame: color target was created for different device.");
  if (target.imageType() != VK_IMAGE_TYPE_2D) Abort("ColorFrameBuilder::createFrame: type of color target is not VK_IMAGE_TYPE_2D");

  glm::uvec2 extent(target.extent().x, target.extent().y);
  std::unique_ptr<ColorFrame> newFrame(new ColorFrame(extent, *this));

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

  Ref<Image> colorBuffer(new Image( VK_IMAGE_TYPE_2D,
                                    commonColorBufferLayout,
                                    VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
                                      VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
                                    0,
                                    target.format(),
                                    target.extent(),
                                    VK_SAMPLE_COUNT_1_BIT,
                                    1,
                                    1,
                                    VK_IMAGE_ASPECT_COLOR_BIT,
                                    _device));

  Ref<ImageView> colorBufferView( new ImageView(*colorBuffer,
                                                VK_IMAGE_VIEW_TYPE_2D,
                                                colorMapping,
                                                colorSubresourceRange));

  Ref<Image> albedoBuffer(new Image(VK_IMAGE_TYPE_2D,
                                    albedoMapLayout,
                                    albedoImageUsage,
                                    0,
                                    albedoMapFormat,
                                    target.extent(),
                                    VK_SAMPLE_COUNT_1_BIT,
                                    1,
                                    1,
                                    VK_IMAGE_ASPECT_COLOR_BIT,
                                    _device));
  Ref<ImageView> albedoMap(new ImageView( *albedoBuffer,
                                          VK_IMAGE_VIEW_TYPE_2D,
                                          colorMapping,
                                          colorSubresourceRange));

  Ref<Image> specularBuffer(new Image(VK_IMAGE_TYPE_2D,
                                      specularMapLayout,
                                      specularImageUsage,
                                      0,
                                      specularMapFormat,
                                      target.extent(),
                                      VK_SAMPLE_COUNT_1_BIT,
                                      1,
                                      1,
                                      VK_IMAGE_ASPECT_COLOR_BIT,
                                      _device));
  Ref<ImageView> specularMap(new ImageView( *specularBuffer,
                                            VK_IMAGE_VIEW_TYPE_2D,
                                            colorMapping,
                                            colorSubresourceRange));

  Ref<Image> normalBuffer(new Image(VK_IMAGE_TYPE_2D,
                                    normalMapLayout,
                                    normalImageUsage,
                                    0,
                                    normalMapFormat,
                                    target.extent(),
                                    VK_SAMPLE_COUNT_1_BIT,
                                    1,
                                    1,
                                    VK_IMAGE_ASPECT_COLOR_BIT,
                                    _device));
  Ref<ImageView> normalMap(new ImageView( *normalBuffer,
                                          VK_IMAGE_VIEW_TYPE_2D,
                                          colorMapping,
                                          colorSubresourceRange));

  Ref<Image> ambientWeightBuffer( new Image(VK_IMAGE_TYPE_2D,
                                            ambientWeightMapLayout,
                                            ambientWeightImageUsage,
                                            0,
                                            ambientWeightFormat,
                                            target.extent(),
                                            VK_SAMPLE_COUNT_1_BIT,
                                            1,
                                            1,
                                            VK_IMAGE_ASPECT_COLOR_BIT,
                                            _device));
  Ref<ImageView> ambientWeightMap(new ImageView(*ambientWeightBuffer,
                                                VK_IMAGE_VIEW_TYPE_2D,
                                                colorMapping,
                                                colorSubresourceRange));

  Ref<Image> depthBuffer(
                    new Image(VK_IMAGE_TYPE_2D,
                              commonDepthBufferLayout,
                              depthImageUsage,
                              0,
                              depthMapFormat,
                              target.extent(),
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

  Ref<ImageView> depthMap(new ImageView(*depthBuffer,
                                        VK_IMAGE_VIEW_TYPE_2D,
                                        colorMapping,
                                        depthSubresourceRange));

  Ref<Image> flDepthSamplerImage(
                    new Image(VK_IMAGE_TYPE_2D,
                              flDepthSamplerLayout,
                              flDepthSamplerUsage,
                              0,
                              depthMapFormat,
                              target.extent(),
                              VK_SAMPLE_COUNT_1_BIT,
                              1,
                              1,
                              VK_IMAGE_ASPECT_DEPTH_BIT,
                              _device));

  Ref<ImageView> flDepthSamplerView(new ImageView(*flDepthSamplerImage,
                                                  VK_IMAGE_VIEW_TYPE_2D,
                                                  colorMapping,
                                                  depthSubresourceRange));

  newFrame->geometryPassFrameBuffer =
                                  _gBufferPass->createFrameBuffer(*depthMap,
                                                                  *normalMap,
                                                                  *albedoMap,
                                                                  *specularMap);

  newFrame->ambietWeightPassFrameBuffer =
            _ambientWeightPass->createFrameBuffer(*ambientWeightMap, *depthMap);

  newFrame->lightingPassFrameBuffer =
                          _lightingPass->createFrameBuffer( *colorBufferView,
                                                            *ambientWeightMap,
                                                            *depthMap,
                                                            *normalMap,
                                                            *albedoMap,
                                                            *specularMap);

  newFrame->emissionPassFrameBuffer =
                            _emissionPass->createFrameBuffer( *colorBufferView,
                                                              *depthMap);

  newFrame->backgroundPassFrameBuffer =
                          _backgroundPass->createFrameBuffer( *colorBufferView,
                                                              *depthMap);

  newFrame->forwardLightPassFrameBuffer =
                    _forwardLightPass->createFrameBuffer( *colorBufferView,
                                                          *depthMap,
                                                          *flDepthSamplerView);

  newFrame->selectionPassFrameBuffer =
                            _selectionPass->createFrameBuffer(*colorBufferView,
                                                              *depthMap);

  newFrame->target = &target;
  newFrame->colorBuffer = colorBuffer.get();
  newFrame->commonDepthBuffer = depthBuffer.get();
  newFrame->flDepthSamplerImage = flDepthSamplerImage.get();

  return newFrame;
}

std::unique_ptr<AbstractFramePlan>
                ColorFrameBuilder::createFramePlan(AbstractFrame& frame) const
{
  if(&frame.builder() != this) Abort("ColorFrameBuilder::createFramePlan: this frame was created by another builder.");
  return std::make_unique<ColorFramePlan>(frame);
}

void ColorFrameBuilder::scheduleRender( AbstractFramePlan& plan,
                                        CommandProducer& drawProducer)
{
  AbstractFrame& target = plan.frame();
  if(&target.builder() != this) Abort("ColorFrameBuilder::scheduleRender: the frame was not created in this renderer.");
  ColorFrame& frame = static_cast<ColorFrame&>(target);

  DrawContext drawContext{drawProducer, nullptr};

  DrawBin* modificatorsPrepareBin = plan.getBin(modificatorsPrepareStage);
  if(modificatorsPrepareBin == nullptr) Abort("ColorFrameBuilder::scheduleRender: modificatorsPrepareBin is null.");
  modificatorsPrepareBin->execute(drawContext);

  drawContext.frameBuffer = frame.geometryPassFrameBuffer.get();
  _gBufferPass->scheduleRender(plan, drawContext);

  drawContext.frameBuffer = frame.ambietWeightPassFrameBuffer.get();
  _ambientWeightPass->scheduleRender(plan, drawContext);

  drawContext.frameBuffer = frame.lightingPassFrameBuffer.get();
  _lightingPass->scheduleRender(plan, drawContext);

  drawContext.frameBuffer = frame.emissionPassFrameBuffer.get();
  _emissionPass->scheduleRender(plan, drawContext);

  drawContext.frameBuffer = frame.backgroundPassFrameBuffer.get();
  _backgroundPass->scheduleRender(plan, drawContext);

  Image::CopyImageChunk copyInfo{};
  copyInfo.srcArrayIndex = 0;
  copyInfo.srcMipLevel = 0;
  copyInfo.srcOffset = glm::uvec3(0);
  copyInfo.dstArrayIndex = 0;
  copyInfo.dstMipLevel = 0;
  copyInfo.dstOffset = glm::uvec3(0);
  copyInfo.extent = frame.target->extent();

  frame.flDepthSamplerImage->scheduleCopy( drawProducer,
                                          *frame.commonDepthBuffer,
                                          VK_IMAGE_ASPECT_DEPTH_BIT,
                                          VK_IMAGE_ASPECT_DEPTH_BIT,
                                          {copyInfo});

  drawContext.frameBuffer = frame.forwardLightPassFrameBuffer.get();
  _forwardLightPass->scheduleRender(plan, drawContext);

  drawContext.frameBuffer = frame.selectionPassFrameBuffer.get();
  _selectionPass->scheduleRender(plan, drawContext);

  frame.target->scheduleCopy( drawProducer,
                              *frame.colorBuffer,
                              VK_IMAGE_ASPECT_COLOR_BIT,
                              VK_IMAGE_ASPECT_COLOR_BIT,
                              {copyInfo});
}
