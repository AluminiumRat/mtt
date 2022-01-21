#include <cstdlib>

#include <mtt/render/CommandQueue/CommandProducer.h>
#include <mtt/render/Pipeline/GraphicsPipeline.h>
#include <mtt/render/LogicalDevice.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

GraphicsPipeline::GraphicsPipeline(
                          AbstractRenderPass& renderPass,
                          const AbstractRenderPass::SubpassInfo& subpassInfo) :
  AbstractPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, renderPass.device()),
  _pass(renderPass),
  _compatabilityIndex(renderPass.compatabilityIndex()),
  _subpassInfo(subpassInfo),
  _topology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST),
  _dynamicScissorViewport(true),
  _rasterizerDiscardEnable(false),
  _polygonMode(VK_POLYGON_MODE_FILL),
  _lineWidth(1.f),
  _cullMode(VK_CULL_MODE_BACK_BIT),
  _frontFace(VK_FRONT_FACE_CLOCKWISE),
  _depthTestEnable(false),
  _depthWriteEnable(false),
  _depthCompareOp(VK_COMPARE_OP_NEVER),
  _depthBoundsTestEnable(false),
  _minDepthBounds(0),
  _maxDepthBounds(1),
  _stencilTestEnable(false),
  _stencilFront{},
  _stencilBack{}
{
  _viewport.x = 0;
  _viewport.y = 0;
  _viewport.width = 256;
  _viewport.height = 256;
  _viewport.minDepth = 0;
  _viewport.maxDepth = 0;

  _scissor.offset.x = 0;
  _scissor.offset.y = 0;
  _scissor.extent.width = 256;
  _scissor.extent.height = 256;

  _stencilFront.failOp = VK_STENCIL_OP_KEEP;
  _stencilFront.passOp = VK_STENCIL_OP_KEEP;
  _stencilFront.depthFailOp = VK_STENCIL_OP_KEEP;
  _stencilFront.compareOp = VK_COMPARE_OP_NEVER;
  _stencilFront.compareMask = 0;
  _stencilFront.writeMask = 0;
  _stencilFront.reference = 0;

  _stencilBack.failOp = VK_STENCIL_OP_KEEP;
  _stencilBack.passOp = VK_STENCIL_OP_KEEP;
  _stencilBack.depthFailOp = VK_STENCIL_OP_KEEP;
  _stencilBack.compareOp = VK_COMPARE_OP_NEVER;
  _stencilBack.compareMask = 0;
  _stencilBack.writeMask = 0;
  _stencilBack.reference = 0;
}

GraphicsPipeline::GraphicsPipeline( AbstractRenderPass& renderPass,
                                    StageIndex stage) :
  AbstractPipeline(VK_PIPELINE_BIND_POINT_GRAPHICS, renderPass.device()),
  _pass(renderPass),
  _compatabilityIndex(renderPass.compatabilityIndex()),
  _subpassInfo(_pass.subpassInfo(stage)),
  _topology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST),
  _dynamicScissorViewport(true),
  _rasterizerDiscardEnable(false),
  _polygonMode(VK_POLYGON_MODE_FILL),
  _lineWidth(1.f),
  _cullMode(VK_CULL_MODE_BACK_BIT),
  _frontFace(VK_FRONT_FACE_CLOCKWISE),
  _depthTestEnable(false),
  _depthWriteEnable(false),
  _depthCompareOp(VK_COMPARE_OP_NEVER),
  _depthBoundsTestEnable(false),
  _minDepthBounds(0),
  _maxDepthBounds(1),
  _stencilTestEnable(false),
  _stencilFront{},
  _stencilBack{}
{
  _viewport.x = 0;
  _viewport.y = 0;
  _viewport.width = 256;
  _viewport.height = 256;
  _viewport.minDepth = 0;
  _viewport.maxDepth = 0;

  _scissor.offset.x = 0;
  _scissor.offset.y = 0;
  _scissor.extent.width = 256;
  _scissor.extent.height = 256;

  _stencilFront.failOp = VK_STENCIL_OP_KEEP;
  _stencilFront.passOp = VK_STENCIL_OP_KEEP;
  _stencilFront.depthFailOp = VK_STENCIL_OP_KEEP;
  _stencilFront.compareOp = VK_COMPARE_OP_NEVER;
  _stencilFront.compareMask = 0;
  _stencilFront.writeMask = 0;
  _stencilFront.reference = 0;

  _stencilBack.failOp = VK_STENCIL_OP_KEEP;
  _stencilBack.passOp = VK_STENCIL_OP_KEEP;
  _stencilBack.depthFailOp = VK_STENCIL_OP_KEEP;
  _stencilBack.compareOp = VK_COMPARE_OP_NEVER;
  _stencilBack.compareMask = 0;
  _stencilBack.writeMask = 0;
  _stencilBack.reference = 0;
}

void GraphicsPipeline::addShader(std::unique_ptr<ShaderModule> shaderModule)
{
  if(shaderModule == nullptr) Abort("GraphicsPipeline::addShader: shader module is null.");

  if (shaderModule->type() == ShaderModule::VERTEX_SHADER)
  {
    for (std::unique_ptr<VertexAttribute>& attribute : _attributes)
    {
      shaderModule->setDefine(attribute->locationName());
    }
  }

  AbstractPipeline::addShader(std::move(shaderModule));
}

std::unique_ptr<ShaderModule> GraphicsPipeline::removeShader(
                                          ShaderModule& shaderModule) noexcept
{
  return AbstractPipeline::removeShader(shaderModule);
}

void GraphicsPipeline::scheduleBind(CommandProducer& drawProducer)
{
  AbstractPipeline::scheduleBind(drawProducer);
  
  for(Attributes::iterator iAttribute = _attributes.begin();
      iAttribute != _attributes.end();
      iAttribute++)
  {
    (*iAttribute)->scheduleBind(drawProducer);
  }

  _indices.scheduleBind(drawProducer);

  if(_dynamicScissorViewport)
  {
    vkCmdSetScissor(drawProducer.bufferHandle(),
                    0,
                    1,
                    &_scissor);
    vkCmdSetViewport( drawProducer.bufferHandle(),
                      0,
                      1,
                      &_viewport);
  }
}

void GraphicsPipeline::rebuild()
{
  invalidate();
  _adjustAttributesLocationsAndBindings();
  AbstractPipeline::rebuild();
}

VkPipeline GraphicsPipeline::createPipeline(
              const std::vector<VkPipelineShaderStageCreateInfo>& shaderStages,
              VkPipelineLayout pipelineLayout)
{
  std::vector<VkVertexInputBindingDescription> bindings;
  std::vector<VkVertexInputAttributeDescription> attributesInfo;
  for (std::unique_ptr<VertexAttribute>& iAttribute : _attributes)
  {
    VertexAttribute& attribute = *iAttribute;
    VkVertexInputBindingDescription bindingInfo{};
    bindingInfo.binding = attribute.binding();
    bindingInfo.stride = attribute.dataStride();
    bindingInfo.inputRate = attribute.inputRate();
    bindings.push_back(bindingInfo);
  
    VkVertexInputAttributeDescription attributeInfo{};
    attributeInfo.binding = attribute.binding();
    attributeInfo.location = attribute.location();
    attributeInfo.format = attribute.format();
    attributeInfo.offset = 0;
    attributesInfo.push_back(attributeInfo);
  }

  VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
  vertexInputInfo.sType =
                    VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertexInputInfo.vertexBindingDescriptionCount = uint32_t(bindings.size());
  vertexInputInfo.pVertexBindingDescriptions = bindings.data();
  vertexInputInfo.vertexAttributeDescriptionCount =
                                                uint32_t(attributesInfo.size());
  vertexInputInfo.pVertexAttributeDescriptions = attributesInfo.data();

  VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
  inputAssembly.sType =
                VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  inputAssembly.topology = _topology;
  inputAssembly.primitiveRestartEnable = VK_FALSE;

  VkPipelineViewportStateCreateInfo viewportState{};
  viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewportState.viewportCount = 1;
  viewportState.pViewports = &_viewport;
  viewportState.scissorCount = 1;
  viewportState.pScissors = &_scissor;

  VkPipelineRasterizationStateCreateInfo rasterizerInfo{};
  rasterizerInfo.sType =
                  VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizerInfo.depthClampEnable = VK_FALSE;
  rasterizerInfo.rasterizerDiscardEnable = _rasterizerDiscardEnable ?
                                                            VK_TRUE : VK_FALSE;
  rasterizerInfo.polygonMode = _polygonMode;
  rasterizerInfo.lineWidth = _lineWidth;
  rasterizerInfo.cullMode = _cullMode;
  rasterizerInfo.frontFace = _frontFace;
  rasterizerInfo.depthBiasEnable = VK_FALSE;
  rasterizerInfo.depthBiasConstantFactor = 0.0f;
  rasterizerInfo.depthBiasClamp = 0.0f;
  rasterizerInfo.depthBiasSlopeFactor = 0.0f;

  VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};
  depthStencilInfo.sType =
                    VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  depthStencilInfo.depthTestEnable = _depthTestEnable ? VK_TRUE : VK_FALSE;
  depthStencilInfo.depthWriteEnable = _depthWriteEnable ? VK_TRUE : VK_FALSE;
  depthStencilInfo.depthCompareOp = _depthCompareOp;
  depthStencilInfo.depthBoundsTestEnable = _depthBoundsTestEnable ?
                                                            VK_TRUE : VK_FALSE;
  depthStencilInfo.stencilTestEnable = _stencilTestEnable ? VK_TRUE : VK_FALSE;
  depthStencilInfo.front = _stencilFront;
  depthStencilInfo.back = _stencilBack;
  depthStencilInfo.minDepthBounds = _minDepthBounds;
  depthStencilInfo.maxDepthBounds = _maxDepthBounds;

  VkPipelineColorBlendStateCreateInfo blendingInfo{};
  blendingInfo.sType =
                    VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  blendingInfo.logicOpEnable = VK_FALSE;
  blendingInfo.logicOp = VK_LOGIC_OP_COPY;
  blendingInfo.attachmentCount = uint32_t(_subpassInfo.blentAttachments.size());
  blendingInfo.pAttachments = _subpassInfo.blentAttachments.data();
  blendingInfo.blendConstants[0] = 0.0f;
  blendingInfo.blendConstants[1] = 0.0f;
  blendingInfo.blendConstants[2] = 0.0f;
  blendingInfo.blendConstants[3] = 0.0f;

  VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
  dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  VkDynamicState dynamicStates[2] = { VK_DYNAMIC_STATE_VIEWPORT,
                                      VK_DYNAMIC_STATE_SCISSOR};
  dynamicStateInfo.dynamicStateCount = 2;
  dynamicStateInfo.pDynamicStates = dynamicStates;

  VkGraphicsPipelineCreateInfo pipelineInfo{};
  pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  
  pipelineInfo.stageCount = uint32_t(shaderStages.size());
  pipelineInfo.pStages = shaderStages.data();

  pipelineInfo.pVertexInputState = &vertexInputInfo;
  pipelineInfo.pInputAssemblyState = &inputAssembly;
  pipelineInfo.pViewportState = &viewportState;
  pipelineInfo.pRasterizationState = &rasterizerInfo;
  pipelineInfo.pMultisampleState = &_subpassInfo.multisampling;
  pipelineInfo.pDepthStencilState = &depthStencilInfo;
  pipelineInfo.pColorBlendState = &blendingInfo;
  pipelineInfo.pDynamicState = _dynamicScissorViewport ?
                                                  &dynamicStateInfo : nullptr;

  pipelineInfo.layout = pipelineLayout;

  pipelineInfo.renderPass = _pass.handle();
  pipelineInfo.subpass = _subpassInfo.subpassIndex;

  pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
  pipelineInfo.basePipelineIndex = -1;

  VkPipeline result = VK_NULL_HANDLE;

  if (vkCreateGraphicsPipelines(device().handle(),
                                VK_NULL_HANDLE,
                                1,
                                &pipelineInfo,
                                nullptr,
                                &result) != VK_SUCCESS)
  {
    throw std::runtime_error("Failed to create graphics pipeline.");
  }

  return result;
}

void GraphicsPipeline::_adjustAttributesLocationsAndBindings()
{
  uint32_t currentLocation = 0;
  uint32_t currentBinding = 0;
  for(std::unique_ptr<VertexAttribute>& iAttribute : _attributes)
  {
    VertexAttribute& attribute = *iAttribute;
    attribute.setLocation(currentLocation);
    attribute.setBinding(currentBinding);

    std::string strLocation = std::to_string(currentLocation);

    for(size_t shaderIndex = 0;
        shaderIndex < shaderNumber();
        shaderIndex++)
    {
      ShaderModule& shaderModule = shader(shaderIndex);
      if(shaderModule.type() == ShaderModule::VERTEX_SHADER)
      {
        shaderModule.setDefine(attribute.locationName(), strLocation);
      }
    }

    currentLocation += attribute.locationSize();
    currentBinding++;
  }
}

VertexAttribute& GraphicsPipeline::getOrCreateAttribute(
                                                const std::string& locationName)
{
  for(Attributes::iterator iAttribute = _attributes.begin();
      iAttribute != _attributes.end();
      iAttribute++)
  {
    if(iAttribute->get()->locationName() == locationName)
    {
      return **iAttribute;
    }
  }

  std::unique_ptr<VertexAttribute> newAttribute(
                                      new VertexAttribute(locationName, *this));
  VertexAttribute& attribute = *newAttribute;

  for(size_t shaderIndex = 0;
      shaderIndex < shaderNumber();
      shaderIndex++)
  {
    ShaderModule& shaderModule = shader(shaderIndex);
    if (shaderModule.type() == ShaderModule::VERTEX_SHADER)
    {
      shaderModule.setDefine(locationName);
    }
  }

  _attributes.push_back(std::move(newAttribute));
  
  invalidate();
  
  return attribute;
}

void GraphicsPipeline::removeAttribute(VertexAttribute& attribute)
{
  for(Attributes::iterator iAttribute = _attributes.begin();
      iAttribute != _attributes.end();
      iAttribute++)
  {
    if(iAttribute->get() == &attribute)
    {
      for(size_t shaderIndex = 0;
          shaderIndex < shaderNumber();
          shaderIndex++)
      {
        ShaderModule& shaderModule = shader(shaderIndex);
        if(shaderModule.type() == ShaderModule::VERTEX_SHADER)
        {
          shaderModule.removeDefine(iAttribute->get()->locationName());
        }
      }

      _attributes.erase(iAttribute);
      invalidate();
    }
  }

  Abort("GraphicsPipeline::removeAttribute: attribute was not found.");
}
