#include <mtt/Render/RenderPass/DepthMapRenderPass.h>

using namespace mtt;

static const uint32_t depthAttachmentIndex = 0;
static const size_t attachmentNumber = 1;

DepthMapRenderPass::DepthMapRenderPass( VkFormat depthMapFormat,
                                        VkImageLayout depthMapLayout,
                                        LogicalDevice& device) :
  GeneralRenderPass(device),
  _depthMapFormat(depthMapFormat),
  _depthMapLayout(depthMapLayout)
{
  std::vector<VkSubpassDependency> dependencies(1);
  dependencies[0] = {};
  dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
  dependencies[0].dstSubpass = 0;
  dependencies[0].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependencies[0].srcAccessMask = 0;
  dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

  VkAttachmentDescription depthAttachment{};
  depthAttachment.format = _depthMapFormat;
  depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.initialLayout = depthMapLayout;
  depthAttachment.finalLayout = depthMapLayout;

  std::vector<AttachmentDescription> attachments(attachmentNumber);
  attachments[depthAttachmentIndex].attachmentInfo = depthAttachment;
  attachments[depthAttachmentIndex].clearValue.depthStencil = { 0.f, 0 };

  std::vector<SubpassDescription> subpasses(1);
  subpasses[0] = {};

  VkAttachmentReference depthAttachmentRef{};
  depthAttachmentRef.attachment = depthAttachmentIndex;
  depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
  subpasses[0].depthStencilAttachment.emplace(depthAttachmentRef);

  subpasses[0].bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

  SubpassInfo subpassInfo;
  subpassInfo.subpassIndex = 0;
  fillMultisampling(subpassInfo);
  subpassInfo.blentAttachments.push_back(noColorBlendingState);
  subpasses[0].subpassInfo = subpassInfo;

  subpasses[0].stages.push_back(baseStages::depthmapStage);

  setup(dependencies, attachments, 0, subpasses);
}

Ref<FrameBuffer> DepthMapRenderPass::createFrameBuffer(ImageView& depthBuffer)
{
  std::vector<ImageView*> attachments(attachmentNumber);
  attachments[depthAttachmentIndex] = &depthBuffer;

  return GeneralRenderPass::createFrameBuffer(attachments, {});
}
