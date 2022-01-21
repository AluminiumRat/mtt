#include <mtt/dlPipeline/RenderPass/BackgroundPass.h>
#include <mtt/dlPipeline/constants.h>

using namespace mtt;
using namespace mtt::dlPipeline;

BackgroundPass::BackgroundPass( StageIndex stage,
                                VkFormat colorBufferFormat,
                                VkImageLayout colorBufferLayout,
                                VkFormat depthMapFormat,
                                VkImageLayout depthMapLayout,
                                LogicalDevice& device) :
  GeneralRenderPass(device),
  _colorBufferFormat(colorBufferFormat),
  _colorBufferLayout(colorBufferLayout),
  _depthMapFormat(depthMapFormat),
  _depthMapLayout(depthMapLayout)
{
  std::vector<VkSubpassDependency> dependencies(1);
  dependencies[0] = {};
  dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
  dependencies[0].dstSubpass = 0;
  dependencies[0].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependencies[0].srcAccessMask = 0;
  dependencies[0].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  dependencies[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

  VkAttachmentDescription colorAttachment{};
  colorAttachment.format = _colorBufferFormat;
  colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
  colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachment.initialLayout = _colorBufferLayout;
  colorAttachment.finalLayout = _colorBufferLayout;

  VkAttachmentDescription depthAttachment{};
  depthAttachment.format = _depthMapFormat;
  depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
  depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.initialLayout = _depthMapLayout;
  depthAttachment.finalLayout = _depthMapLayout;

  std::vector<AttachmentDescription> attachments(attachmentNumber);
  attachments[colorAttachmentIndex].attachmentInfo = colorAttachment;
  attachments[colorAttachmentIndex].clearValue = { 0.f, 0.f, 0.f, 1.f };
  attachments[depthMapAttachmentIndex].attachmentInfo = depthAttachment;
  attachments[depthMapAttachmentIndex].clearValue.depthStencil = {0.f, 0};

  std::vector<SubpassDescription> subpasses(1);
  subpasses[0] = {};
  subpasses[0].colorAttachments.resize(1);
  subpasses[0].colorAttachments[0] = {};
  subpasses[0].colorAttachments[0].attachment = colorAttachmentIndex;
  subpasses[0].colorAttachments[0].layout =
                                      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  subpasses[0].inputAttachments.resize(1);
  subpasses[0].inputAttachments[0] = {};
  subpasses[0].inputAttachments[0].attachment = depthMapAttachmentIndex;
  subpasses[0].inputAttachments[0].layout =
                                      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

  subpasses[0].bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

  SubpassInfo subpassInfo;
  subpassInfo.subpassIndex = 0;
  fillMultisampling(subpassInfo);

  VkPipelineColorBlendAttachmentState blendingState{};
  blendingState.blendEnable = VK_TRUE;
  if(stage == backgroundStage)
  {
    blendingState.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
  }
  else
  {
    blendingState.srcColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
  }
  blendingState.dstColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
  blendingState.colorBlendOp = VK_BLEND_OP_ADD;
  blendingState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
  blendingState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
  blendingState.alphaBlendOp = VK_BLEND_OP_ADD;
  blendingState.colorWriteMask =  VK_COLOR_COMPONENT_R_BIT |
                                  VK_COLOR_COMPONENT_G_BIT |
                                  VK_COLOR_COMPONENT_B_BIT;

  subpassInfo.blentAttachments.push_back(blendingState);
  subpasses[0].subpassInfo = subpassInfo;

  subpasses[0].stages.push_back(stage);

  setup(dependencies, attachments, 0, subpasses);
}

Ref<FrameBuffer> BackgroundPass::createFrameBuffer( ImageView& colorBuffer,
                                                    ImageView& depthMap)
{
  std::vector<ImageView*> attachments(attachmentNumber);
  attachments[colorAttachmentIndex] = &colorBuffer;
  attachments[depthMapAttachmentIndex] = &depthMap;

  return GeneralRenderPass::createFrameBuffer(attachments, {});
}
