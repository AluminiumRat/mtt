#include <mtt/DrawImpl/RenderPass/EmissionPass.h>
#include <mtt/DrawImpl/constants.h>

using namespace mtt;

static const uint32_t colorAttachmentIndex = 0;
static const uint32_t depthAttachmentIndex = 1;
static const size_t attachmentNumber = 2;

EmissionPass::EmissionPass( VkAttachmentLoadOp colorBufferLoadOp,
                            VkFormat colorBufferFormat,
                            VkImageLayout colorBufferLayout,
                            VkFormat depthBufferFormat,
                            VkImageLayout depthBufferLayout,
                            LogicalDevice& device) :
  GeneralRenderPass(device),
  _colorBufferLoadOp(colorBufferLoadOp),
  _colorBufferFormat(colorBufferFormat),
  _colorBufferLayout(colorBufferLayout),
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

  VkAttachmentDescription colorAttachment{};
  colorAttachment.format = _colorBufferFormat;
  colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  colorAttachment.loadOp = _colorBufferLoadOp;
  colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachment.initialLayout = _colorBufferLayout;
  colorAttachment.finalLayout = _colorBufferLayout;

  VkAttachmentDescription depthAttachment{};
  depthAttachment.format = _depthBufferFormat;
  depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
  depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.initialLayout = _depthBufferLayout;
  depthAttachment.finalLayout = _depthBufferLayout;

  std::vector<AttachmentDescription> attachments(attachmentNumber);
  attachments[colorAttachmentIndex].attachmentInfo = colorAttachment;
  attachments[colorAttachmentIndex].clearValue = { 0.f, 0.f, 0.f, 1.f };
  attachments[depthAttachmentIndex].attachmentInfo = depthAttachment;
  attachments[depthAttachmentIndex].clearValue.depthStencil = { 0.f, 0 };

  std::vector<SubpassDescription> subpasses(1);
  subpasses[0] = {};
  subpasses[0].colorAttachments.resize(1);
  subpasses[0].colorAttachments[0] = {};
  subpasses[0].colorAttachments[0].attachment = colorAttachmentIndex;
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
  subpassInfo.blentAttachments.push_back(accamulateColorBlendingState);
  subpasses[0].subpassInfo = subpassInfo;

  subpasses[0].stages.push_back(emissionStage);

  setup(dependencies, attachments, 0, subpasses);
}

Ref<FrameBuffer> EmissionPass::createFrameBuffer( ImageView& colorTarget,
                                                  ImageView& depthMap)
{
  std::vector<ImageView*> attachments(attachmentNumber);
  attachments[colorAttachmentIndex] = &colorTarget;
  attachments[depthAttachmentIndex] = &depthMap;

  return GeneralRenderPass::createFrameBuffer(attachments, {});
}
