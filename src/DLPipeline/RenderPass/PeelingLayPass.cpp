#include <mtt/DLPipeline/RenderPass/PeelingLayPass.h>
#include <mtt/DLPipeline/constants.h>

using namespace mtt;
using namespace mtt::DLPipeline;

static constexpr uint32_t targetAttachmentIndex = 0;
static constexpr uint32_t accumulatorAttachmentIndex = 1;
static constexpr size_t attachmentNumber = 2;

PeelingLayPass::PeelingLayPass( VkFormat accumulatorFormat,
                                VkImageLayout accumulatorLayout,
                                VkFormat targetFormat,
                                VkImageLayout targetLayout,
                                LogicalDevice& device) :
  AbstractRenderPass(device),
  _accumulatorFormat(accumulatorFormat),
  _accumulatorLayout(accumulatorLayout),
  _targetFormat(targetFormat),
  _targetLayout(targetLayout),
  _accumulatorAttachment(device)
{
  _adjustRenderPass();
  _adjustPipeline();
}

void PeelingLayPass::_adjustRenderPass()
{
  std::vector<VkSubpassDependency> dependencies(1);
  dependencies[0] = {};
  dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
  dependencies[0].dstSubpass = 0;
  dependencies[0].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependencies[0].srcAccessMask = 0;
  dependencies[0].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  dependencies[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

  VkAttachmentDescription targetAttachment{};
  targetAttachment.format = _targetFormat;
  targetAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  targetAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
  targetAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  targetAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  targetAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  targetAttachment.initialLayout = _targetLayout;VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  targetAttachment.finalLayout = _targetLayout;VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkAttachmentDescription accumulatorAttachment{};
  accumulatorAttachment.format = _accumulatorFormat;
  accumulatorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  accumulatorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
  accumulatorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  accumulatorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  accumulatorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  accumulatorAttachment.initialLayout = _accumulatorLayout;
                                      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  accumulatorAttachment.finalLayout = _accumulatorLayout;VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  std::vector<AttachmentDescription> attachments(attachmentNumber);
  attachments[targetAttachmentIndex].attachmentInfo = targetAttachment;
  attachments[targetAttachmentIndex].clearValue = { 0.f, 0.f, 0.f, 1.f };
  attachments[accumulatorAttachmentIndex].attachmentInfo =
                                                          accumulatorAttachment;
  attachments[accumulatorAttachmentIndex].clearValue = { 0.f, 0.f, 0.f, 1.f };

  std::vector<SubpassDescription> subpasses(1);
  subpasses[0] = {};
  subpasses[0].colorAttachments.resize(1);
  subpasses[0].colorAttachments[0] = {};
  subpasses[0].colorAttachments[0].attachment = targetAttachmentIndex;
  subpasses[0].colorAttachments[0].layout =
                                      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  subpasses[0].inputAttachments.resize(1);
  subpasses[0].inputAttachments[0] = {};
  subpasses[0].inputAttachments[0].attachment = accumulatorAttachmentIndex;
  subpasses[0].inputAttachments[0].layout =
                                      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  

  subpasses[0].bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

  VkPipelineColorBlendAttachmentState blendState
  {
    VK_TRUE,                              // blendEnable
    VK_BLEND_FACTOR_ONE,                  // srcColorBlendFactor
    VK_BLEND_FACTOR_SRC_ALPHA,            // dstColorBlendFactor
    VK_BLEND_OP_ADD,                      // colorBlendOp
    VK_BLEND_FACTOR_ZERO,                 // srcAlphaBlendFactor
    VK_BLEND_FACTOR_ONE,                  // dstAlphaBlendFactor
    VK_BLEND_OP_ADD,                      // alphaBlendOp
    VK_COLOR_COMPONENT_R_BIT |            // colorWriteMask;
     VK_COLOR_COMPONENT_G_BIT |
     VK_COLOR_COMPONENT_B_BIT |
     VK_COLOR_COMPONENT_A_BIT
  };

  SubpassInfo subpassInfo;
  subpassInfo.blentAttachments.push_back(blendState);
  subpassInfo.subpassIndex = 0;
  fillMultisampling(subpassInfo);
  subpasses[0].subpassInfo = subpassInfo;

  subpasses[0].stages.push_back(subsidiaryStage);

  setup(dependencies, attachments, 0, subpasses);
}

void PeelingLayPass::_adjustPipeline()
{
  _pipeline.emplace(*this, subsidiaryStage);
  _pipeline->addResource( "accumulatorInputBinding",
                          _accumulatorAttachment,
                          VK_SHADER_STAGE_FRAGMENT_BIT);

  std::unique_ptr<ShaderModule> vertexShader(
                                new ShaderModule( ShaderModule::VERTEX_SHADER,
                                                  device()));
  vertexShader->newFragment().loadFromFile("filter2D.vert");
  _pipeline->addShader(std::move(vertexShader));

  std::unique_ptr<ShaderModule> fragmentShader(
                                new ShaderModule( ShaderModule::FRAGMENT_SHADER,
                                                  device()));
  fragmentShader->newFragment().loadFromFile("DLPipeline/peelingLay.frag");
  _pipeline->addShader(std::move(fragmentShader));

  _pipeline->setTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP);
}

Ref<FrameBuffer> PeelingLayPass::createFrameBuffer( ImageView& targetImage,
                                                    ImageView& accumulatorImage)
{
  std::vector<ImageView*> attachments(attachmentNumber);
  attachments[targetAttachmentIndex] = &targetImage;
  attachments[accumulatorAttachmentIndex] = &accumulatorImage;

  return AbstractRenderPass::createFrameBuffer(attachments, {});
}

void PeelingLayPass::scheduleDraw(DrawContext& context)
{
  _accumulatorAttachment.setAttachment(
                  &context.frameBuffer->attachment(accumulatorAttachmentIndex));

  VkViewport viewport;
  viewport.x = 0.f;
  viewport.y = 0.f;
  viewport.width = float(context.frameBuffer->extent().x);
  viewport.height = float(context.frameBuffer->extent().y);
  viewport.minDepth = 0.f;
  viewport.maxDepth = 1.f;
  _pipeline->setViewport(viewport);

  VkRect2D scissor;
  scissor.offset = { 0,0 };
  scissor.extent = {context.frameBuffer->extent().x,
                    context.frameBuffer->extent().y };
  _pipeline->setScissor(scissor);

  auto drawDelegate =
    [&](DrawContext& context, size_t subpassIndex)
    {
      _pipeline->scheduleBind(context.drawProducer);
      vkCmdDraw(context.drawProducer.bufferHandle(), 4, 1, 0, 0);
    };

  scheduleDrawT(context, drawDelegate);
}
