#include <algorithm>
#include <stdexcept>

#include <mtt/Render/RenderPass/AbstractRenderPass.h>
#include <mtt/Render/LogicalDevice.h>
#include <mtt/Render/RenderLibInstance.h>
#include <mtt/Utilities/Abort.h>

using namespace mtt;

const VkPipelineColorBlendAttachmentState
  AbstractRenderPass::noColorBlendingState
  {
    VK_FALSE,                             // blendEnable
    VK_BLEND_FACTOR_ONE,                  // srcColorBlendFactor
    VK_BLEND_FACTOR_ZERO,                 // dstColorBlendFactor
    VK_BLEND_OP_ADD,                      // colorBlendOp
    VK_BLEND_FACTOR_ONE,                  // srcAlphaBlendFactor
    VK_BLEND_FACTOR_ZERO,                 // dstAlphaBlendFactor
    VK_BLEND_OP_ADD,                      // alphaBlendOp
    VK_COLOR_COMPONENT_R_BIT |            // colorWriteMask;
     VK_COLOR_COMPONENT_G_BIT |
     VK_COLOR_COMPONENT_B_BIT |
     VK_COLOR_COMPONENT_A_BIT
  };

const VkPipelineColorBlendAttachmentState
  AbstractRenderPass::premultipledColorBlendingState
  {
    VK_TRUE,                              // blendEnable
    VK_BLEND_FACTOR_ONE,                  // srcColorBlendFactor
    VK_BLEND_FACTOR_SRC_ALPHA,            // dstColorBlendFactor
    VK_BLEND_OP_ADD,                      // colorBlendOp
    VK_BLEND_FACTOR_ONE,                  // srcAlphaBlendFactor
    VK_BLEND_FACTOR_ZERO,                 // dstAlphaBlendFactor
    VK_BLEND_OP_ADD,                      // alphaBlendOp
    VK_COLOR_COMPONENT_R_BIT |            // colorWriteMask;
     VK_COLOR_COMPONENT_G_BIT |
     VK_COLOR_COMPONENT_B_BIT |
     VK_COLOR_COMPONENT_A_BIT
  };

const VkPipelineColorBlendAttachmentState
  AbstractRenderPass::accamulateColorBlendingState
  {
    VK_TRUE,                              // blendEnable
    VK_BLEND_FACTOR_ONE,                  // srcColorBlendFactor
    VK_BLEND_FACTOR_ONE,                  // dstColorBlendFactor
    VK_BLEND_OP_ADD,                      // colorBlendOp
    VK_BLEND_FACTOR_DST_ALPHA,            // srcAlphaBlendFactor
    VK_BLEND_FACTOR_ZERO,                 // dstAlphaBlendFactor
    VK_BLEND_OP_ADD,                      // alphaBlendOp
    VK_COLOR_COMPONENT_R_BIT |            // colorWriteMask;
     VK_COLOR_COMPONENT_G_BIT |
     VK_COLOR_COMPONENT_B_BIT |
     VK_COLOR_COMPONENT_A_BIT
  };

AbstractRenderPass::AbstractRenderPass(LogicalDevice& device) :
  _handle(VK_NULL_HANDLE),
  _device(device),
  _samplerMapsNumber(0),
  _compatibilityIndex(0)
{
}

AbstractRenderPass::~AbstractRenderPass()
{
  _cleanup();
}

void AbstractRenderPass::_cleanup() noexcept
{
  if(_handle != VK_NULL_HANDLE)
  {
    vkDestroyRenderPass(_device.handle(), _handle, nullptr);
    _handle = VK_NULL_HANDLE;
  }
}

void AbstractRenderPass::setup(
                      const std::vector<VkSubpassDependency>& dependencies,
                      const std::vector<AttachmentDescription>& attachments,
                      size_t samplerMapsNumber,
                      const std::vector<SubpassDescription>& subpasses)
{
  _cleanup();
  _compatibilityIndex = 0;
  try
  {
    _dependencies = dependencies;

    _attachmentsInfo.reserve(attachments.size());
    _clearValues.reserve(attachments.size());
    for(const AttachmentDescription& attachment : attachments)
    {
      _attachmentsInfo.push_back(attachment.attachmentInfo);
      _clearValues.push_back(attachment.clearValue);
    }

    _samplerMapsNumber = samplerMapsNumber;

    _subpasses = subpasses;
    _updateCompatibility();
  }
  catch(...)
  {
    _dependencies.clear();
    _attachmentsInfo.clear();
    _clearValues.clear();
    _subpasses.clear();
    _compatibilityIndex = 0;
  }
}

void AbstractRenderPass::_updateCompatibility()
{
  std::string compatibilityString;

  for(const VkAttachmentDescription& attachment : _attachmentsInfo)
  {
    std::string attachmentString = "AttFl";
    attachmentString += std::to_string(int(attachment.flags));
    attachmentString += "Fr";
    attachmentString += std::to_string(int(attachment.format));
    attachmentString += "Sm";
    attachmentString += std::to_string(int(attachment.samples));
    compatibilityString += attachmentString;
  }

  compatibilityString += "SmN";
  compatibilityString += std::to_string(_samplerMapsNumber);

  for(const SubpassDescription& subpass : _subpasses)
  {
    std::string subpassString = "SbpFl";
    subpassString += std::to_string(int(subpass.flags));
    subpassString += "Bp";
    subpassString += std::to_string(int(subpass.bindPoint));
    for(const VkAttachmentReference& inputAttachment : subpass.inputAttachments)
    {
      subpassString += "Ia";
      subpassString += std::to_string(inputAttachment.attachment);
    }
    for (const VkAttachmentReference& colorAttachment :
                                                      subpass.colorAttachments)
    {
      subpassString += "Ca";
      subpassString += std::to_string(colorAttachment.attachment);
    }
    for (const VkAttachmentReference& resolveAttachment :
                                                    subpass.resolveAttachments)
    {
      subpassString += "Ra";
      subpassString += std::to_string(resolveAttachment.attachment);
    }
    if(subpass.depthStencilAttachment.has_value())
    {
      subpassString += "Da";
      subpassString += std::to_string(
                                  subpass.depthStencilAttachment->attachment);
    }
    compatibilityString += subpassString;
  }

  _compatibilityIndex =
      RenderLibInstance::instance().namesRegistry.getIndex(compatibilityString);
}

void AbstractRenderPass::_createHandle() const
{
  VkRenderPassCreateInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount = uint32_t(_attachmentsInfo.size());
  if(!_attachmentsInfo.empty())
  {
    renderPassInfo.pAttachments = _attachmentsInfo.data();
  }
  renderPassInfo.dependencyCount = uint32_t(_dependencies.size());
  if(!_dependencies.empty())
  {
    renderPassInfo.pDependencies = _dependencies.data();
  }

  renderPassInfo.subpassCount = uint32_t(_subpasses.size());
  std::vector<VkSubpassDescription> subpassesDescriptions;
  if(!_subpasses.empty())
  {
    for(const SubpassDescription& subpass : _subpasses)
    {
      VkSubpassDescription subpassDescription{};
      subpassDescription.flags = subpass.bindPoint;
      subpassDescription.pipelineBindPoint = subpass.bindPoint;
      subpassDescription.inputAttachmentCount =
                                      uint32_t(subpass.inputAttachments.size());
      if(!subpass.inputAttachments.empty())
      {
        subpassDescription.pInputAttachments = subpass.inputAttachments.data();
      }

      subpassDescription.colorAttachmentCount =
                                      uint32_t(subpass.colorAttachments.size());
      if(!subpass.colorAttachments.empty())
      {
        subpassDescription.pColorAttachments = subpass.colorAttachments.data();
        if(!subpass.resolveAttachments.empty())
        {
          if(subpass.resolveAttachments.size() !=
                                                subpass.colorAttachments.size())
          {
            Abort("AbstractRenderPass::_createHandle: wrong resolve attachments count.");
          }
          subpassDescription.pResolveAttachments =
                                              subpass.resolveAttachments.data();
        }
      }

      if(subpass.depthStencilAttachment.has_value())
      {
        subpassDescription.pDepthStencilAttachment =
                                        &subpass.depthStencilAttachment.value();
      }

      subpassDescription.preserveAttachmentCount =
                                  uint32_t(subpass.preserveAttachments.size());
      if(!subpass.preserveAttachments.empty())
      {
        subpassDescription.pPreserveAttachments =
                                            subpass.preserveAttachments.data();
      }

      subpassesDescriptions.push_back(subpassDescription);
    }
    
    renderPassInfo.pSubpasses = subpassesDescriptions.data();
  }
  
  if (vkCreateRenderPass( device().handle(),
                          &renderPassInfo,
                          nullptr,
                          &_handle) != VK_SUCCESS)
  {
    throw std::runtime_error("Failed to create render pass.");
  }
}

bool AbstractRenderPass::stageSupports(StageIndex stage) const noexcept
{
  for(const SubpassDescription& subpass : _subpasses)
  {
    if(std::find( subpass.stages.begin(),
                  subpass.stages.end(),
                  stage) != subpass.stages.end()) return true;
  }
  return false;
}

const AbstractRenderPass::SubpassInfo& AbstractRenderPass::subpassInfo(
                                                StageIndex stage) const noexcept
{
  for(const SubpassDescription& subpass : _subpasses)
  {
    if(std::find( subpass.stages.begin(),
                  subpass.stages.end(),
                  stage) != subpass.stages.end()) return subpass.subpassInfo;
  }
  Abort("AbstractRenderPass::subpassInfo: stage is not supported.");
}

void AbstractRenderPass::fillMultisampling(SubpassInfo& info) const noexcept
{
  info.multisampling = VkPipelineMultisampleStateCreateInfo{};
  info.multisampling.sType =
                      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  info.multisampling.sampleShadingEnable = VK_FALSE;
  info.multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  info.multisampling.minSampleShading = 1.0f;
  info.multisampling.pSampleMask = nullptr;
  info.multisampling.alphaToCoverageEnable = VK_FALSE;
  info.multisampling.alphaToOneEnable = VK_FALSE;
}

Ref<FrameBuffer> AbstractRenderPass::createFrameBuffer(
                                    const std::vector<ImageView*>& attachments,
                                    const std::vector<ImageView*>& samplerMaps)
{
  if(attachments.size() != _attachmentsInfo.size()) Abort("AbstractRenderPass::createFrameBuffer: wrong number of attachments.");
  if(samplerMaps.size() != _samplerMapsNumber) Abort("AbstractRenderPass::createFrameBuffer: wrong number of sampler maps.");

  if(attachments.size() != 0)
  {
    glm::uvec3 extent = attachments[0]->extent();

    for(size_t attachmentIndex = 0;
        attachmentIndex < attachments.size();
        attachmentIndex++)
    {
      if(attachments[attachmentIndex]->viewType() != VK_IMAGE_VIEW_TYPE_2D) Abort("AbstractRenderPass::createFrameBuffer: attachment type is not VK_IMAGE_VIEW_TYPE_2D");
      if(attachments[attachmentIndex]->extent() != extent) Abort("AbstractRenderPass::createFrameBuffer: attachments extent does not match.");
      if(attachments[attachmentIndex]->image().format() !=
                                          _attachmentsInfo[attachmentIndex].format)
      {
        Log() << "AbstractRenderPass::createFrameBuffer: attachment index: " << attachmentIndex;
        Log() << "AbstractRenderPass::createFrameBuffer: attachment format is: " << attachments[attachmentIndex]->image().format();
        Log() << "AbstractRenderPass::createFrameBuffer: expected format is: " << _attachmentsInfo[attachmentIndex].format;
        Abort("AbstractRenderPass::createFrameBuffer: wrong attachment format.");
      }
      if(attachments[attachmentIndex]->image().primaryLayout() !=
                                _attachmentsInfo[attachmentIndex].initialLayout)
      {
        Log() << "AbstractRenderPass::createFrameBuffer: attachment index: " << attachmentIndex;
        Log() << "AbstractRenderPass::createFrameBuffer: attachment layout is: " << attachments[attachmentIndex]->image().primaryLayout();
        Log() << "AbstractRenderPass::createFrameBuffer: expected layout is: " << _attachmentsInfo[attachmentIndex].initialLayout;
        Abort("AbstractRenderPass::createFrameBuffer: wrong attachment layout.");
      }
    }
  }

  return Ref<FrameBuffer>(new FrameBuffer(*this, attachments, samplerMaps));
}
