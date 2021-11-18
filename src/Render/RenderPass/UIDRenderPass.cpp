#include <mtt/Render/RenderPass/UIDRenderPass.h>

using namespace mtt;

static const uint32_t uidAttachmentIndex = 0;
static const uint32_t depthAttachmentIndex = 1;
static const size_t attachmentNumber = 2;

UIDRenderPass::UIDRenderPass( VkFormat uidMapFormat,
                              VkImageLayout uidMapLayout,
                              VkFormat depthBufferFormat,
                              VkImageLayout depthBufferLayout,
                              LogicalDevice& device) :
  GeneralRenderPass(device),
  _uidMapFormat(uidMapFormat),
  _uidMapLayout(uidMapLayout),
  _depthBufferFormat(depthBufferFormat),
  _depthBufferLayout(depthBufferLayout)
{
  std::vector<VkSubpassDependency> dependencies(1);
  dependencies[0] = {};
  dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
  dependencies[0].dstSubpass = 0;
  dependencies[0].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependencies[0].srcAccessMask = 0;
  dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

  VkAttachmentDescription uidAttachment{};
  uidAttachment.format = _uidMapFormat;
  uidAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  uidAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  uidAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  uidAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  uidAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  uidAttachment.initialLayout = _uidMapLayout;
  uidAttachment.finalLayout = _uidMapLayout;

  VkAttachmentDescription depthAttachment{};
  depthAttachment.format = _depthBufferFormat;
  depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.initialLayout = _depthBufferLayout;
  depthAttachment.finalLayout = _depthBufferLayout;

  std::vector<AttachmentDescription> attachments(attachmentNumber);
  attachments[uidAttachmentIndex].attachmentInfo = uidAttachment;
  attachments[uidAttachmentIndex].clearValue = { 0.f, 0.f, 0.f, 0.f };
  attachments[depthAttachmentIndex].attachmentInfo = depthAttachment;
  attachments[depthAttachmentIndex].clearValue.depthStencil = { 0.f, 0 };

  std::vector<SubpassDescription> subpasses(1);
  subpasses[0] = {};
  subpasses[0].colorAttachments.resize(1);
  subpasses[0].colorAttachments[0] = {};
  subpasses[0].colorAttachments[0].attachment = uidAttachmentIndex;
  subpasses[0].colorAttachments[0].layout =
                                      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

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

  subpasses[0].stages.push_back(baseStages::uidStage);

  setup(dependencies, attachments, 0, subpasses);
}

Ref<FrameBuffer> UIDRenderPass::createFrameBuffer(ImageView& uidBuffer,
                                                  ImageView& depthBuffer)
{
  std::vector<ImageView*> attachments(attachmentNumber);
  attachments[uidAttachmentIndex] = &uidBuffer;
  attachments[depthAttachmentIndex] = &depthBuffer;

  return GeneralRenderPass::createFrameBuffer(attachments, {});
}
