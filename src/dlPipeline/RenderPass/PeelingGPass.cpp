#include <mtt/dlPipeline/RenderPass/PeelingGPass.h>
#include <mtt/dlPipeline/constants.h>

using namespace mtt;
using namespace mtt::dlPipeline;

PeelingGPass::PeelingGPass( VkFormat depthMapFormat,
                            VkImageLayout depthMapLayout,
                            VkFormat normalMapFormat,
                            VkImageLayout normalMapLayout,
                            VkFormat albedoMapFormat,
                            VkImageLayout albedoMapLayout,
                            VkFormat specularMapFormat,
                            VkImageLayout specularMapLayout,
                            VkFormat frontMapFormat,
                            VkImageLayout frontMapLayout,
                            VkFormat backMapFormat,
                            VkImageLayout backMapLayout,
                            LogicalDevice& device) :
  GeneralRenderPass(device),
  _depthMapFormat(depthMapFormat),
  _depthMapLayout(depthMapLayout),
  _normalMapFormat(normalMapFormat),
  _normalMapLayout(normalMapLayout),
  _albedoMapFormat(albedoMapFormat),
  _albedoMapLayout(albedoMapLayout),
  _specularMapFormat(specularMapFormat),
  _specularMapLayout(specularMapLayout),
  _frontMapFormat(frontMapFormat),
  _frontMapLayout(frontMapLayout),
  _backMapFormat(backMapFormat),
  _backMapLayout(backMapLayout)
{
  std::vector<VkSubpassDependency> dependencies(1);
  dependencies[0] = {};
  dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
  dependencies[0].dstSubpass = 0;
  dependencies[0].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependencies[0].srcAccessMask = 0;
  dependencies[0].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  dependencies[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

  VkAttachmentDescription depthAttachment{};
  depthAttachment.format = _depthMapFormat;
  depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.initialLayout = _depthMapLayout;
  depthAttachment.finalLayout = _depthMapLayout;

  VkAttachmentDescription albedoAttachment{};
  albedoAttachment.format = _albedoMapFormat;
  albedoAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  albedoAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  albedoAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  albedoAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  albedoAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  albedoAttachment.initialLayout = _albedoMapLayout;
  albedoAttachment.finalLayout = _albedoMapLayout;

  VkAttachmentDescription specularAttachment{};
  specularAttachment.format = _specularMapFormat;
  specularAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  specularAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  specularAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  specularAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  specularAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  specularAttachment.initialLayout = _specularMapLayout;
  specularAttachment.finalLayout = _specularMapLayout;

  VkAttachmentDescription normalAttachment{};
  normalAttachment.format = _normalMapFormat;
  normalAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  normalAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  normalAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  normalAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  normalAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  normalAttachment.initialLayout = _normalMapLayout;
  normalAttachment.finalLayout = _normalMapLayout;

  VkAttachmentDescription frontAttachment{};
  frontAttachment.format = _frontMapFormat;
  frontAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  frontAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
  frontAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  frontAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  frontAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  frontAttachment.initialLayout = _frontMapLayout;
  frontAttachment.finalLayout = _frontMapLayout;

  VkAttachmentDescription backAttachment{};
  backAttachment.format = _backMapFormat;;
  backAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  backAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
  backAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  backAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  backAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  backAttachment.initialLayout = _backMapLayout;
  backAttachment.finalLayout = _backMapLayout;

  std::vector<AttachmentDescription> attachments(attachmentNumber);
  attachments[depthAttachmentIndex].attachmentInfo = depthAttachment;
  attachments[depthAttachmentIndex].clearValue.depthStencil = { 0.f, 0 };
  attachments[albedoAttachmentIndex].attachmentInfo = albedoAttachment;
  attachments[albedoAttachmentIndex].clearValue = { 1.f, 1.f, 1.f, 1.f };
  attachments[specularAttachmentIndex].attachmentInfo = specularAttachment;
  attachments[specularAttachmentIndex].clearValue = { 0.f, 0.f, 0.f, 0.f };
  attachments[normalAttachmentIndex].attachmentInfo = normalAttachment;
  attachments[normalAttachmentIndex].clearValue = { 0.f, 0.f, 0.f, 0.f };
  attachments[frontAttachmentIndex].attachmentInfo = frontAttachment;
  attachments[frontAttachmentIndex].clearValue = { 0.f, 0.f, 0.f, 0.f };
  attachments[backAttachmentIndex].attachmentInfo = backAttachment;
  attachments[backAttachmentIndex].clearValue = { 0.f, 0.f, 0.f, 0.f };

  std::vector<SubpassDescription> subpasses(1);
  subpasses[0] = {};
  subpasses[0].colorAttachments.resize(3);
  subpasses[0].colorAttachments[0] = {};
  subpasses[0].colorAttachments[0].attachment = albedoAttachmentIndex;
  subpasses[0].colorAttachments[0].layout =
                                      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  subpasses[0].colorAttachments[1] = {};
  subpasses[0].colorAttachments[1].attachment = specularAttachmentIndex;
  subpasses[0].colorAttachments[1].layout =
                                      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  subpasses[0].colorAttachments[2] = {};
  subpasses[0].colorAttachments[2].attachment = normalAttachmentIndex;
  subpasses[0].colorAttachments[2].layout =
                                      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  subpasses[0].inputAttachments.resize(2);
  subpasses[0].inputAttachments[0] = {};
  subpasses[0].inputAttachments[0].attachment = frontAttachmentIndex;
  subpasses[0].inputAttachments[0].layout =
                                      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  subpasses[0].inputAttachments[1] = {};
  subpasses[0].inputAttachments[1].attachment = backAttachmentIndex;
  subpasses[0].inputAttachments[1].layout =
                                      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

  VkAttachmentReference depthAttachmentRef{};
  depthAttachmentRef.attachment = depthAttachmentIndex;
  depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
  subpasses[0].depthStencilAttachment.emplace(depthAttachmentRef);

  subpasses[0].bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

  SubpassInfo subpassInfo;
  subpassInfo.subpassIndex = 0;
  fillMultisampling(subpassInfo);
  subpassInfo.blentAttachments.push_back(noColorBlendingState);
  subpassInfo.blentAttachments.push_back(noColorBlendingState);
  subpassInfo.blentAttachments.push_back(noColorBlendingState);
  subpasses[0].subpassInfo = subpassInfo;

  subpasses[0].stages.push_back(peelingGBufferStage);

  setup(dependencies, attachments, 0, subpasses);
}

Ref<FrameBuffer> PeelingGPass::createFrameBuffer( ImageView& depthMap,
                                                  ImageView& normalMap,
                                                  ImageView& albedoMap,
                                                  ImageView& specularMap,
                                                  ImageView& frontMap,
                                                  ImageView& backMap)
{
  std::vector<ImageView*> attachments(attachmentNumber);
  attachments[depthAttachmentIndex] = &depthMap;
  attachments[normalAttachmentIndex] = &normalMap;
  attachments[albedoAttachmentIndex] = &albedoMap;
  attachments[specularAttachmentIndex] = &specularMap;
  attachments[frontAttachmentIndex] = &frontMap;
  attachments[backAttachmentIndex] = &backMap;

  return GeneralRenderPass::createFrameBuffer(attachments, {});
}
