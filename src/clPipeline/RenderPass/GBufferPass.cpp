#include <mtt/clPipeline/RenderPass/GBufferPass.h>
#include <mtt/clPipeline/constants.h>

using namespace mtt;
using namespace mtt::clPipeline;

static const uint32_t depthAttachmentIndex = 0;
static const uint32_t normalAttachmentIndex = 1;
static const uint32_t albedoAttachmentIndex = 2;
static const uint32_t specularAttachmentIndex = 3;
static const size_t attachmentNumber = 4;

GBufferPass::GBufferPass( VkFormat depthMapFormat,
                          VkImageLayout depthMapLayout,
                          VkFormat normalMapFormat,
                          VkImageLayout normalMapLayout,
                          VkFormat albedoMapFormat,
                          VkImageLayout albedoMapLayout,
                          VkFormat specularMapFormat,
                          VkImageLayout specularMapLayout,
                          LogicalDevice& device) :
  _depthMapFormat(depthMapFormat),
  _depthMapLayout(depthMapLayout),
  _normalMapFormat(normalMapFormat),
  _normalMapLayout(normalMapLayout),
  _albedoMapFormat(albedoMapFormat),
  _albedoMapLayout(albedoMapLayout),
  _specularMapFormat(specularMapFormat),
  _specularMapLayout(specularMapLayout),
  GeneralRenderPass(device)
{
  std::vector<VkSubpassDependency> dependencies(0);

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

  std::vector<AttachmentDescription> attachments(attachmentNumber);
  attachments[depthAttachmentIndex].attachmentInfo = depthAttachment;
  attachments[depthAttachmentIndex].clearValue.depthStencil = { 0.f, 0 };
  attachments[albedoAttachmentIndex].attachmentInfo = albedoAttachment;
  attachments[albedoAttachmentIndex].clearValue = { 1.f, 1.f, 1.f, 1.f };
  attachments[specularAttachmentIndex].attachmentInfo = specularAttachment;
  attachments[specularAttachmentIndex].clearValue = { 0.f, 0.f, 0.f, 0.f };
  attachments[normalAttachmentIndex].attachmentInfo = normalAttachment;
  attachments[normalAttachmentIndex].clearValue = { 0.f, 0.f, 1.f, 0.f };

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

  subpasses[0].stages.push_back(gBufferStage);

  setup(dependencies, attachments, 0, subpasses);
}

Ref<FrameBuffer> GBufferPass::createFrameBuffer(ImageView& depthMap,
                                                ImageView& normalMap,
                                                ImageView& albedoMap,
                                                ImageView& specularMap)
{
  std::vector<ImageView*> attachments(attachmentNumber);
  attachments[depthAttachmentIndex] = &depthMap;
  attachments[normalAttachmentIndex] = &normalMap;
  attachments[albedoAttachmentIndex] = &albedoMap;
  attachments[specularAttachmentIndex] = &specularMap;

  return GeneralRenderPass::createFrameBuffer(attachments, {});
}
