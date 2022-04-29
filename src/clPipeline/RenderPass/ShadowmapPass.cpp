#include <mtt/clPipeline/RenderPass/ShadowmapPass.h>
#include <mtt/clPipeline/constants.h>

using namespace mtt;
using namespace mtt::clPipeline;

ShadowmapPass::ShadowmapPass( VkFormat shadowmapFormat,
                              VkImageLayout shadowmapLayout,
                              VkFormat depthmapFormat,
                              VkImageLayout depthmapLayout,
                              mtt::StageIndex stageIndex,
                              LogicalDevice& device) :
  GeneralRenderPass(device),
  _shadowmapFormat(shadowmapFormat),
  _shadowmapLayout(shadowmapLayout),
  _depthmapFormat(depthmapFormat),
  _depthmapLayout(depthmapLayout),
  _stageIndex(stageIndex)
{
  std::vector<VkSubpassDependency> dependencies(1);
  dependencies[0] = {};
  dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
  dependencies[0].dstSubpass = 0;
  dependencies[0].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependencies[0].srcAccessMask = 0;
  dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

  VkAttachmentDescription mapAttachment{};
  mapAttachment.format = _shadowmapFormat;
  mapAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  if (stageIndex == opaqueShadowmapStage)
  {
    mapAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  }
  else mapAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
  mapAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  mapAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  mapAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  mapAttachment.initialLayout = _shadowmapLayout;
  mapAttachment.finalLayout = _shadowmapLayout;

  VkAttachmentDescription depthAttachment{};
  depthAttachment.format = _depthmapFormat;
  depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  if(stageIndex == opaqueShadowmapStage)
  {
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  }
  else depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
  depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.initialLayout = _depthmapLayout;
  depthAttachment.finalLayout = _depthmapLayout;

  std::vector<AttachmentDescription> attachments(attachmentNumber);
  attachments[shadowmapAttachmentIndex].attachmentInfo = mapAttachment;
  attachments[shadowmapAttachmentIndex].clearValue = { 1.f, 0.f, 0.f, 0.f };
  attachments[depthAttachmentIndex].attachmentInfo = depthAttachment;
  attachments[depthAttachmentIndex].clearValue.depthStencil = { 0.f, 0 };

  std::vector<SubpassDescription> subpasses(1);
  subpasses[0] = {};
  subpasses[0].colorAttachments.resize(1);
  subpasses[0].colorAttachments[0] = {};
  subpasses[0].colorAttachments[0].attachment = shadowmapAttachmentIndex;
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
  if (stageIndex == opaqueShadowmapStage)
  {
    VkPipelineColorBlendAttachmentState blendingState = noColorBlendingState;
    blendingState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT;
    subpassInfo.blentAttachments.push_back(blendingState);
  }
  else
  {
    VkPipelineColorBlendAttachmentState blendingState
    {
      VK_TRUE,                              // blendEnable
      VK_BLEND_FACTOR_SRC_ALPHA,            // srcColorBlendFactor
      VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,  // dstColorBlendFactor
      VK_BLEND_OP_ADD,                      // colorBlendOp
      VK_BLEND_FACTOR_ONE,                  // srcAlphaBlendFactor
      VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,  // dstAlphaBlendFactor
      VK_BLEND_OP_ADD,                      // alphaBlendOp
      VK_COLOR_COMPONENT_G_BIT |            // colorWriteMask;
       VK_COLOR_COMPONENT_B_BIT |
       VK_COLOR_COMPONENT_A_BIT
    };
    subpassInfo.blentAttachments.push_back(blendingState);
  }
  subpasses[0].subpassInfo = subpassInfo;

  subpasses[0].stages.push_back(_stageIndex);

  setup(dependencies, attachments, 0, subpasses);
}

Ref<FrameBuffer> ShadowmapPass::createFrameBuffer(ImageView& shadowMapBuffer,
                                                  ImageView& depthBuffer)
{
  std::vector<ImageView*> attachments(attachmentNumber);
  attachments[shadowmapAttachmentIndex] = &shadowMapBuffer;
  attachments[depthAttachmentIndex] = &depthBuffer;

  return GeneralRenderPass::createFrameBuffer(attachments, {});
}
