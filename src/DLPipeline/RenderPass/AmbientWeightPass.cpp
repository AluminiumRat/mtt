#include <mtt/DLPipeline/RenderPass/AmbientWeightPass.h>

using namespace mtt;
using namespace mtt::DLPipeline;

static constexpr uint32_t weightAttachmentIndex = 0;
static constexpr size_t attachmentNumber = 1;

AmbientWeightPass::AmbientWeightPass( StageIndex stage,
                                      VkFormat weightBufferFormat,
                                      VkImageLayout weightBufferLayout,
                                      LogicalDevice& device) :
  GeneralRenderPass(device),
  _weightBufferFormat(weightBufferFormat),
  _weightBufferLayout(weightBufferLayout)
{
  std::vector<VkSubpassDependency> dependencies(1);
  dependencies[0] = {};
  dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
  dependencies[0].dstSubpass = 0;
  dependencies[0].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependencies[0].srcAccessMask = 0;
  dependencies[0].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  dependencies[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

  VkAttachmentDescription weightAttachment{};
  weightAttachment.format = _weightBufferFormat;
  weightAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  weightAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  weightAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  weightAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  weightAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  weightAttachment.initialLayout = _weightBufferLayout;
  weightAttachment.finalLayout = _weightBufferLayout;

  std::vector<AttachmentDescription> attachments(attachmentNumber);
  attachments[weightAttachmentIndex].attachmentInfo = weightAttachment;
  attachments[weightAttachmentIndex].clearValue = { 0.f, 0.f, 0.f, 1.f };

  std::vector<SubpassDescription> subpasses(1);
  subpasses[0] = {};
  subpasses[0].colorAttachments.resize(1);
  subpasses[0].colorAttachments[0] = {};
  subpasses[0].colorAttachments[0].attachment = weightAttachmentIndex;
  subpasses[0].colorAttachments[0].layout =
                                      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  subpasses[0].bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

  SubpassInfo subpassInfo;
  subpassInfo.subpassIndex = 0;
  fillMultisampling(subpassInfo);
  subpassInfo.blentAttachments.push_back(accamulateColorBlendingState);
  subpasses[0].subpassInfo = subpassInfo;

  subpasses[0].stages.push_back(stage);

  setup(dependencies, attachments, samplerMapNumber, subpasses);
}

Ref<FrameBuffer> AmbientWeightPass::createFrameBuffer(ImageView& weightTarget,
                                                      ImageView& depthMap)
{
  std::vector<ImageView*> attachments(attachmentNumber);
  attachments[weightAttachmentIndex] = &weightTarget;

  std::vector<ImageView*> samplerMaps(samplerMapNumber);
  samplerMaps[depthSamplerMapIndex] = &depthMap;

  return GeneralRenderPass::createFrameBuffer(attachments, samplerMaps);
}
