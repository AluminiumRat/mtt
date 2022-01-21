#include <mtt/dlPipeline/RenderPass/LightingPass.h>

using namespace mtt;
using namespace mtt::dlPipeline;

static constexpr uint32_t colorAttachmentIndex = 0;
static constexpr size_t attachmentNumber = 1;

LightingPass::LightingPass( StageIndex stage,
                            VkAttachmentLoadOp colorBufferLoadOp,
                            VkFormat colorBufferFormat,
                            VkImageLayout colorBufferLayout,
                            LogicalDevice& device) :
  GeneralRenderPass(device),
  _colorBufferLoadOp(colorBufferLoadOp),
  _colorBufferFormat(colorBufferFormat),
  _colorBufferLayout(colorBufferLayout)
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
  colorAttachment.loadOp = _colorBufferLoadOp;
  colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachment.initialLayout = _colorBufferLayout;
  colorAttachment.finalLayout = _colorBufferLayout;

  std::vector<AttachmentDescription> attachments(attachmentNumber);
  attachments[colorAttachmentIndex].attachmentInfo = colorAttachment;
  attachments[colorAttachmentIndex].clearValue = { 0.f, 0.f, 0.f, 1.f };

  std::vector<SubpassDescription> subpasses(1);
  subpasses[0] = {};
  subpasses[0].colorAttachments.resize(1);
  subpasses[0].colorAttachments[0] = {};
  subpasses[0].colorAttachments[0].attachment = colorAttachmentIndex;
  subpasses[0].colorAttachments[0].layout =
                                      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  subpasses[0].bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

  SubpassInfo subpassInfo;
  subpassInfo.subpassIndex = 0;
  fillMultisampling(subpassInfo);
  VkPipelineColorBlendAttachmentState blendingState
  {
    VK_TRUE,                              // blendEnable
    VK_BLEND_FACTOR_ONE,                  // srcColorBlendFactor
    VK_BLEND_FACTOR_ONE,                  // dstColorBlendFactor
    VK_BLEND_OP_ADD,                      // colorBlendOp
    VK_BLEND_FACTOR_ONE,                  // srcAlphaBlendFactor
    VK_BLEND_FACTOR_ZERO,                 // dstAlphaBlendFactor
    VK_BLEND_OP_ADD,                      // alphaBlendOp
    VK_COLOR_COMPONENT_R_BIT |            // colorWriteMask;
     VK_COLOR_COMPONENT_G_BIT |
     VK_COLOR_COMPONENT_B_BIT |
     VK_COLOR_COMPONENT_A_BIT
  };
  subpassInfo.blentAttachments.push_back(blendingState);
  subpasses[0].subpassInfo = subpassInfo;

  subpasses[0].stages.push_back(stage);

  setup(dependencies, attachments, samplerMapNumber, subpasses);
}

Ref<FrameBuffer> LightingPass::createFrameBuffer( ImageView& colorTarget,
                                                  ImageView& weightMap,
                                                  ImageView& depthMap,
                                                  ImageView& normalMap,
                                                  ImageView& albedoMap,
                                                  ImageView& specularMap)
{
  std::vector<ImageView*> attachments(attachmentNumber);
  attachments[colorAttachmentIndex] = &colorTarget;
  
  std::vector<ImageView*> samplerMaps(samplerMapNumber);
  samplerMaps[weightSamplerMapIndex] = &weightMap;
  samplerMaps[depthSamplerMapIndex] = &depthMap;
  samplerMaps[normalSamplerMapIndex] = &normalMap;
  samplerMaps[albedoSamplerMapIndex] = &albedoMap;
  samplerMaps[specularSamplerMapIndex] = &specularMap;

  return GeneralRenderPass::createFrameBuffer(attachments, samplerMaps);
}
