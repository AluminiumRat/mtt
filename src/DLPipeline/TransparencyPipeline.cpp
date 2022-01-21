#include <mtt/DLPipeline/ColorFrameBuilder.h>
#include <mtt/DLPipeline/TransparencyPipeline.h>
#include <mtt/DLPipeline/constants.h>
#include <mtt/Render/ImageView.h>
#include <mtt/Utilities/Abort.h>

using namespace mtt;
using namespace mtt::DLPipeline;

static constexpr VkImageUsageFlags depthImageUsage =
                                  VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT |
                                  VK_IMAGE_USAGE_SAMPLED_BIT |
                                  VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;

static constexpr VkImageUsageFlags normalImageUsage =
                                          VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
                                          VK_IMAGE_USAGE_SAMPLED_BIT |
                                          VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;

static constexpr VkImageUsageFlags albedoImageUsage =
                                          VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
                                          VK_IMAGE_USAGE_SAMPLED_BIT |
                                          VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;

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

TransparencyPipeline::TransparencyPipeline(
                                          PeelingGPass& gBufferPass,
                                          AmbientWeightPass& ambientWeightPass,
                                          LightingPass& lightingPass,
                                          BackgroundPass& backgroundPass,
                                          size_t stepsNumber) :
  _gBufferPass(gBufferPass),
  _ambientWeightPass(ambientWeightPass),
  _lightingPass(lightingPass),
  _backgroundPass(backgroundPass),
  _mergePass(new PeelingMergePass(lightingPass.colorBufferFormat(),
                                  lightingPass.colorBufferLayout(),
                                  lightingPass.colorBufferFormat(),
                                  lightingPass.colorBufferLayout(),
                                  lightingPass.device())),
  _layPass(new PeelingLayPass(lightingPass.colorBufferFormat(),
                              lightingPass.colorBufferLayout(),
                              lightingPass.colorBufferFormat(),
                              lightingPass.colorBufferLayout(),
                              lightingPass.device())),
  _stepsNumber(stepsNumber)
{
}

TransparencyPipeline::FrameData TransparencyPipeline::createFrameData(
                                                      ImageView& target,
                                                      ImageView& frameDepthMap)
{
  if(target.viewType() != VK_IMAGE_VIEW_TYPE_2D) Abort("TransparencyPipeline::createFrameData: wrong target view type.");
  if(frameDepthMap.viewType() != VK_IMAGE_VIEW_TYPE_2D) Abort("TransparencyPipeline::createFrameData: wrong depth map view type.");

  LogicalDevice& device = _gBufferPass.device();

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

  Ref<Image> albedoBuffer(new Image(VK_IMAGE_TYPE_2D,
                                    _gBufferPass.albedoMapLayout(),
                                    albedoImageUsage,
                                    0,
                                    _gBufferPass.albedoMapFormat(),
                                    target.extent(),
                                    VK_SAMPLE_COUNT_1_BIT,
                                    1,
                                    1,
                                    VK_IMAGE_ASPECT_COLOR_BIT,
                                    device));
  Ref<ImageView> albedoMap( new ImageView(*albedoBuffer,
                                          VK_IMAGE_VIEW_TYPE_2D,
                                          colorMapping,
                                          colorSubresourceRange));
  
  Ref<Image> specularBuffer(new Image(VK_IMAGE_TYPE_2D,
                                      _gBufferPass.specularMapLayout(),
                                      specularImageUsage,
                                      0,
                                      _gBufferPass.specularMapFormat(),
                                      target.extent(),
                                      VK_SAMPLE_COUNT_1_BIT,
                                      1,
                                      1,
                                      VK_IMAGE_ASPECT_COLOR_BIT,
                                      device));
  Ref<ImageView> specularMap( new ImageView(*specularBuffer,
                                            VK_IMAGE_VIEW_TYPE_2D,
                                            colorMapping,
                                            colorSubresourceRange));
  
  Ref<Image> normalBuffer(new Image(VK_IMAGE_TYPE_2D,
                                    _gBufferPass.normalMapLayout(),
                                    normalImageUsage,
                                    0,
                                    _gBufferPass.normalMapFormat(),
                                    target.extent(),
                                    VK_SAMPLE_COUNT_1_BIT,
                                    1,
                                    1,
                                    VK_IMAGE_ASPECT_COLOR_BIT,
                                    device));
  Ref<ImageView> normalMap(new ImageView( *normalBuffer,
                                          VK_IMAGE_VIEW_TYPE_2D,
                                          colorMapping,
                                          colorSubresourceRange));

  Ref<Image> ambientWeightBuffer(
                              new Image(VK_IMAGE_TYPE_2D,
                                        ambientWeightMapLayout,
                                        ambientWeightImageUsage,
                                        0,
                                        ColorFrameBuilder::ambientWeightFormat,
                                        target.extent(),
                                        VK_SAMPLE_COUNT_1_BIT,
                                        1,
                                        1,
                                        VK_IMAGE_ASPECT_COLOR_BIT,
                                        device));
  Ref<ImageView> ambientWeightMap(new ImageView(*ambientWeightBuffer,
                                                VK_IMAGE_VIEW_TYPE_2D,
                                                colorMapping,
                                                colorSubresourceRange));

  Ref<Image> depthBuffer1(new Image(VK_IMAGE_TYPE_2D,
                                    _gBufferPass.depthMapLayout(),
                                    depthImageUsage,
                                    0,
                                    _gBufferPass.depthMapFormat(),
                                    target.extent(),
                                    VK_SAMPLE_COUNT_1_BIT,
                                    1,
                                    1,
                                    VK_IMAGE_ASPECT_DEPTH_BIT,
                                    device));

  Ref<Image> depthBuffer2(new Image(VK_IMAGE_TYPE_2D,
                                    _gBufferPass.depthMapLayout(),
                                    depthImageUsage | 
                                      VK_IMAGE_USAGE_TRANSFER_DST_BIT,
                                    0,
                                    _gBufferPass.depthMapFormat(),
                                    target.extent(),
                                    VK_SAMPLE_COUNT_1_BIT,
                                    1,
                                    1,
                                    VK_IMAGE_ASPECT_DEPTH_BIT,
                                    device));

  VkImageSubresourceRange depthSubresourceRange{};
  depthSubresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
  depthSubresourceRange.baseMipLevel = 0;
  depthSubresourceRange.levelCount = 1;
  depthSubresourceRange.baseArrayLayer = 0;
  depthSubresourceRange.layerCount = 1;

  Ref<ImageView> depthMap1(new ImageView( *depthBuffer1,
                                          VK_IMAGE_VIEW_TYPE_2D,
                                          colorMapping,
                                          depthSubresourceRange));

  Ref<ImageView> depthMap2(new ImageView( *depthBuffer2,
                                          VK_IMAGE_VIEW_TYPE_2D,
                                          colorMapping,
                                          depthSubresourceRange));

  Ref<Image> lightTargetBuffer(
                              new Image(VK_IMAGE_TYPE_2D,
                                        _lightingPass.colorBufferLayout(),
                                        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
                                          VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
                                        0,
                                        _lightingPass.colorBufferFormat(),
                                        target.extent(),
                                        VK_SAMPLE_COUNT_1_BIT,
                                        1,
                                        1,
                                        VK_IMAGE_ASPECT_COLOR_BIT,
                                        device));
  Ref<ImageView> lightTargetMap(new ImageView(*lightTargetBuffer,
                                              VK_IMAGE_VIEW_TYPE_2D,
                                              colorMapping,
                                              colorSubresourceRange));

  Ref<Image> mergeTargetBuffer(
                            new Image(VK_IMAGE_TYPE_2D,
                                      _lightingPass.colorBufferLayout(),
                                      VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
                                        VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT |
                                        VK_IMAGE_USAGE_TRANSFER_DST_BIT,
                                      0,
                                      _lightingPass.colorBufferFormat(),
                                      target.extent(),
                                      VK_SAMPLE_COUNT_1_BIT,
                                      1,
                                      1,
                                      VK_IMAGE_ASPECT_COLOR_BIT,
                                      device));
  Ref<ImageView> mergeTargetMap(new ImageView(*mergeTargetBuffer,
                                              VK_IMAGE_VIEW_TYPE_2D,
                                              colorMapping,
                                              colorSubresourceRange));

  FrameData frameData{};
  frameData.gPassFrameBuffers[0] = _gBufferPass.createFrameBuffer(
                                                                *depthMap1,
                                                                *normalMap,
                                                                *albedoMap,
                                                                *specularMap,
                                                                *depthMap2,
                                                                frameDepthMap);
  frameData.gPassFrameBuffers[1] = _gBufferPass.createFrameBuffer(
                                                                *depthMap2,
                                                                *normalMap,
                                                                *albedoMap,
                                                                *specularMap,
                                                                *depthMap1,
                                                                frameDepthMap);

  frameData.ambientWeightFrameBuffers[0] =
                        _ambientWeightPass.createFrameBuffer( *ambientWeightMap,
                                                              *depthMap1);

  frameData.ambientWeightFrameBuffers[1] =
                        _ambientWeightPass.createFrameBuffer( *ambientWeightMap,
                                                              *depthMap2);

  frameData.lightingFrameBuffer[0] = _lightingPass.createFrameBuffer(
                                                              *lightTargetMap,
                                                              *ambientWeightMap,
                                                              *depthMap1,
                                                              *normalMap,
                                                              *albedoMap,
                                                              *specularMap);
  frameData.lightingFrameBuffer[1] = _lightingPass.createFrameBuffer(
                                                              *lightTargetMap,
                                                              *ambientWeightMap,
                                                              *depthMap2,
                                                              *normalMap,
                                                              *albedoMap,
                                                              *specularMap);

  frameData.backgroundFrameBuffer[0] = _backgroundPass.createFrameBuffer(
                                                                *lightTargetMap,
                                                                *depthMap1);
  frameData.backgroundFrameBuffer[1] = _backgroundPass.createFrameBuffer(
                                                                *lightTargetMap,
                                                                *depthMap2);

  frameData.mergeFrameBuffer = _mergePass->createFrameBuffer( *mergeTargetMap,
                                                              *lightTargetMap);

  frameData.layFrameBuffer = _layPass->createFrameBuffer( target,
                                                          *mergeTargetMap);
  
  frameData.firstFrontBuffer = depthBuffer2.get();
  frameData.accumulator = mergeTargetBuffer.get();
  
  return frameData;
}

void TransparencyPipeline::scheduleDraw(FrameData& frameData,
                                        AbstractFramePlan& plan,
                                        DrawContext& drawContext)
{
  DrawBin* renderBin = plan.getBin(peelingGBufferStage);
  if(renderBin == nullptr) Abort("TransparencyPipeline::scheduleDraw: peeling G buffer render bin is not supported.");
  if(renderBin->empty()) return;

  Image::ClearChunk clearInfo{0, 0};
  VkClearValue clearValue;
  clearValue.depthStencil = {1.f, 0};
  frameData.firstFrontBuffer->scheduleClear(drawContext.drawProducer,
                                            VK_IMAGE_ASPECT_DEPTH_BIT,
                                            clearValue,
                                            {clearInfo});

  clearValue.color = { 0.f, 0.f, 0.f, 1.f };
  frameData.accumulator->scheduleClear( drawContext.drawProducer,
                                        VK_IMAGE_ASPECT_COLOR_BIT,
                                        clearValue,
                                        {clearInfo});

  for(size_t stepIndex = 0; stepIndex < _stepsNumber; stepIndex++)
  {
    drawContext.frameBuffer = frameData.gPassFrameBuffers[stepIndex % 2].get();
    _gBufferPass.scheduleRender(plan, drawContext);

    drawContext.frameBuffer =
                      frameData.ambientWeightFrameBuffers[stepIndex % 2].get();
    _ambientWeightPass.scheduleRender(plan, drawContext);

    drawContext.frameBuffer =
                            frameData.lightingFrameBuffer[stepIndex % 2].get();
    _lightingPass.scheduleRender(plan, drawContext);

    drawContext.frameBuffer =
                          frameData.backgroundFrameBuffer[stepIndex % 2].get();
    _backgroundPass.scheduleRender(plan, drawContext);

    drawContext.frameBuffer = frameData.mergeFrameBuffer.get();
    _mergePass->scheduleDraw(drawContext);
  }

  drawContext.frameBuffer = frameData.layFrameBuffer.get();
  _layPass->scheduleDraw(drawContext);

  drawContext.frameBuffer = nullptr;
}
