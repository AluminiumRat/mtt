#include <mtt/render/RenderPass/AbstractFilter.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

static const size_t attachmentNumber = 1;
static const uint32_t targetAttachmentIndex = 0;

AbstractFilter::AbstractFilter( VkFormat targetFormat,
                                VkImageLayout targetLayout,
                                LogicalDevice& device) :
  AbstractRenderPass(device),
  _targetFormat(targetFormat),
  _targetLayout(targetLayout),
  _srcSampler(PipelineResource::VOLATILE, device),
  _srcTexture(new Texture2D(device))
{
  _adjustRenderPass();
  _adjustPipeline();
}

void AbstractFilter::_adjustRenderPass()
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
  targetAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  targetAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  targetAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  targetAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  targetAttachment.initialLayout = _targetLayout;
  targetAttachment.finalLayout = _targetLayout;

  std::vector<AttachmentDescription> attachments(attachmentNumber);
  attachments[targetAttachmentIndex].attachmentInfo = targetAttachment;
  attachments[targetAttachmentIndex].clearValue = { 0.f, 0.f, 0.f, 1.f };

  std::vector<SubpassDescription> subpasses(1);
  subpasses[0] = {};
  subpasses[0].colorAttachments.resize(1);
  subpasses[0].colorAttachments[0] = {};
  subpasses[0].colorAttachments[0].attachment = targetAttachmentIndex;
  subpasses[0].colorAttachments[0].layout =
                                      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  subpasses[0].bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

  SubpassInfo subpassInfo;
  subpassInfo.blentAttachments.push_back(noColorBlendingState);
  subpassInfo.subpassIndex = 0;
  fillMultisampling(subpassInfo);
  subpasses[0].subpassInfo = subpassInfo;

  subpasses[0].stages.push_back(baseStages::subsidiaryStage);

  setup(dependencies, attachments, 0, subpasses);
}

void AbstractFilter::_adjustPipeline()
{
  _pipeline.emplace(*this, baseStages::subsidiaryStage);
  _pipeline->addResource( "srcSamplerBinding",
                          _srcSampler,
                          VK_SHADER_STAGE_FRAGMENT_BIT);
  _srcSampler.setAttachedTexture(_srcTexture);

  std::unique_ptr<ShaderModule> vertexShader(
                                  new ShaderModule( ShaderModule::VERTEX_SHADER,
                                                    device()));
  vertexShader->newFragment().loadFromFile("filter2D.vert");
  _pipeline->addShader(std::move(vertexShader));

  _pipeline->setTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP);
}

Ref<FrameBuffer> AbstractFilter::createFrameBuffer(ImageView& target)
{
  if(target.viewType() != VK_IMAGE_VIEW_TYPE_2D) Abort("AbstractFilter::createFrameBuffer: wrong target view type.");

  if(target.image().format() != _targetFormat) Abort("AbstractFilter::createFrameBuffer: wrong target format.");
  if(target.image().primaryLayout() != _targetLayout) Abort("AbstractFilter::createFrameBuffer: wrong target primary layout.");

  std::vector<ImageView*> attachments(attachmentNumber);
  attachments[targetAttachmentIndex] = &target;

  return AbstractRenderPass::createFrameBuffer(attachments, {});
}

void AbstractFilter::scheduleDraw(ImageView& srcImage, DrawContext& context)
{
  _srcTexture->setImageView(srcImage);

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

  context.drawProducer.lockResource(srcImage);
}
