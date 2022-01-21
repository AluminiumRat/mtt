#include <mtt/render/Pipeline/AbstractPipeline.h>
#include <mtt/render/Pipeline/PipelineResourceSet.h>
#include <mtt/render/Pipeline/VolatileUniform.h>
#include <mtt/render/Utilities/DescriptorCounter.h>
#include <mtt/render/LogicalDevice.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

PipelineResourceSet::LayoutHandles::~LayoutHandles()
{
  if (commonDescriptorSetLayout != VK_NULL_HANDLE)
  {
    vkDestroyDescriptorSetLayout(device, commonDescriptorSetLayout, nullptr);
    commonDescriptorSetLayout = VK_NULL_HANDLE;
  }
  if (volatileDescriptorSetLayout != VK_NULL_HANDLE)
  {
    vkDestroyDescriptorSetLayout(device, volatileDescriptorSetLayout, nullptr);
    volatileDescriptorSetLayout = VK_NULL_HANDLE;
  }
  if(pipelineLayout != VK_NULL_HANDLE)
  {
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    pipelineLayout = VK_NULL_HANDLE;
  }
}

PipelineResourceSet::DescriptorHandles::~DescriptorHandles()
{
  if (descriptorPool != VK_NULL_HANDLE)
  {
    vkDestroyDescriptorPool(device, descriptorPool, nullptr);
    descriptorPool = VK_NULL_HANDLE;
  }
}

PipelineResourceSet::PipelineResourceSet( AbstractPipeline& pipeline,
                                          VkPipelineBindPoint bindPoint,
                                          LogicalDevice& device) :
  _pipeline(pipeline),
  _device(device),
  _bindPoint(bindPoint)
{
}

void PipelineResourceSet::_invalidate() noexcept
{
  _descriptorHandles.reset();
  _layoutHandles.reset();
  _pipeline.invalidate();
}

void PipelineResourceSet::addResource(PipelineResource& resource,
                                      VkShaderStageFlags stageFlags,
                                      const std::string& bindingName)
{
  Binding newBinding{};
  newBinding.resource = &resource;
  newBinding.stageFlags = stageFlags;
  newBinding.bindingName = bindingName;
  newBinding.lastRevision = 0;

  if(resource.type() == PipelineResource::VOLATILE)
  {
    for (const Binding& binding : _volatileResources)
    {
      if (binding.bindingName == bindingName) Abort("PipelineResourceSet::addResource: binding with this name is already registered.");
    }
    _volatileResources.push_back(newBinding);
    _volatileDescriptorNumber.add(resource.descriptorType(),
                                  resource.descriptorCount());
  }
  else
  {
    for(const Binding& binding : _staticResources)
    {
      if(binding.bindingName == bindingName) Abort("PipelineResourceSet::addResource: binding with this name is already registered.");
    }
    _staticResources.push_back(newBinding);
  }

  _invalidate();
}

void PipelineResourceSet::_removeStaticResource(size_t index) noexcept
{
  _invalidate();
  _staticResources.erase(_staticResources.begin() + index);
}

void PipelineResourceSet::_removeVolatileResource(size_t index) noexcept
{
  _invalidate();
  _volatileDescriptorNumber.reduce(
                        _volatileResources[index].resource->descriptorType(),
                        _volatileResources[index].resource->descriptorCount());
  _volatileResources.erase(_volatileResources.begin() + index);
}

void PipelineResourceSet::_buildLayouts()
{
  _layoutHandles.reset();
  _descriptorHandles.reset();

  Ref<LayoutHandles> newHandles(new LayoutHandles);
  newHandles->device = _device.handle();

  _buildCommonSetLayout(*newHandles);
  _buildVolatileSetLayout(*newHandles);

  std::vector<VkDescriptorSetLayout> setLayouts;
  if(newHandles->commonDescriptorSetLayout != VK_NULL_HANDLE)
  {
    setLayouts.push_back(newHandles->commonDescriptorSetLayout);
  }
  if (newHandles->volatileDescriptorSetLayout != VK_NULL_HANDLE)
  {
    setLayouts.push_back(newHandles->volatileDescriptorSetLayout);
  }

  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = uint32_t(setLayouts.size());
  pipelineLayoutInfo.pSetLayouts = setLayouts.empty() ? nullptr :
                                                        setLayouts.data();
  pipelineLayoutInfo.pushConstantRangeCount = 0;
  pipelineLayoutInfo.pPushConstantRanges = nullptr;
  if (vkCreatePipelineLayout( _device.handle(),
                              &pipelineLayoutInfo,
                              nullptr,
                              &newHandles->pipelineLayout) != VK_SUCCESS)
  {
    throw std::runtime_error("Failed to create pipeline layout.");
  }

  _layoutHandles = std::move(newHandles);
}

void PipelineResourceSet::_buildCommonSetLayout(LayoutHandles& target)
{
  if (_staticResources.empty()) return;

  std::vector<VkDescriptorSetLayoutBinding> commonBindingInfoSet;
  for(size_t bindIndex = 0; bindIndex < _staticResources.size(); bindIndex++)
  {
    const Binding& binding = _staticResources[bindIndex];
    VkDescriptorSetLayoutBinding bindingInfo{};
    bindingInfo.binding = uint32_t(bindIndex);
    bindingInfo.descriptorCount = binding.resource->descriptorCount();
    bindingInfo.descriptorType = binding.resource->descriptorType();
    bindingInfo.stageFlags = binding.stageFlags;
    commonBindingInfoSet.push_back(bindingInfo);
  }

  VkDescriptorSetLayoutCreateInfo setLayoutInfo{};
  setLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  setLayoutInfo.bindingCount = uint32_t(commonBindingInfoSet.size());
  setLayoutInfo.pBindings = commonBindingInfoSet.data();
  if(vkCreateDescriptorSetLayout(
                        _device.handle(),
                        &setLayoutInfo,
                        nullptr,
                        &target.commonDescriptorSetLayout) != VK_SUCCESS)
  {
    throw std::runtime_error("Failed to create common descriptor set layout.");
  }
}

void PipelineResourceSet::_buildVolatileSetLayout(LayoutHandles& target)
{
  if (_volatileResources.empty()) return;

  std::vector<VkDescriptorSetLayoutBinding> volatileBindingInfoSet;
  for(size_t bindIndex = 0; bindIndex < _volatileResources.size(); bindIndex++)
  {
    const Binding& binding = _volatileResources[bindIndex];
    VkDescriptorSetLayoutBinding bindingInfo{};
    bindingInfo.binding = uint32_t(bindIndex);
    bindingInfo.descriptorCount = binding.resource->descriptorCount();
    bindingInfo.descriptorType = binding.resource->descriptorType();
    bindingInfo.stageFlags = binding.stageFlags;
    volatileBindingInfoSet.push_back(bindingInfo);
  }

  VkDescriptorSetLayoutCreateInfo setLayoutInfo{};
  setLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  setLayoutInfo.bindingCount = uint32_t(volatileBindingInfoSet.size());
  setLayoutInfo.pBindings = volatileBindingInfoSet.data();
  if(vkCreateDescriptorSetLayout(
                        _device.handle(),
                        &setLayoutInfo,
                        nullptr,
                        &target.volatileDescriptorSetLayout) != VK_SUCCESS)
  {
    throw std::runtime_error("Failed to create volatile descriptor set layout.");
  }
}

void PipelineResourceSet::_buildDescriptors()
{
  _descriptorHandles.reset();

  if(_staticResources.empty()) return;

  Ref<DescriptorHandles> newHandles(new DescriptorHandles);
  newHandles->device = _device.handle();

  DescriptorCounter descriptorCounter;
  for (size_t bindIndex = 0; bindIndex < _staticResources.size(); bindIndex++)
  {
    const Binding& binding = _staticResources[bindIndex];
    descriptorCounter.add(binding.resource->descriptorType(),
                          binding.resource->descriptorCount());
  }

  std::vector<VkDescriptorPoolSize> sizeInfo =
                                              descriptorCounter.makeSizeInfo();
  VkDescriptorPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  poolInfo.poolSizeCount = uint32_t(sizeInfo.size());
  poolInfo.pPoolSizes = sizeInfo.empty() ? nullptr : sizeInfo.data();
  poolInfo.maxSets = 1;
  if (vkCreateDescriptorPool( _device.handle(),
                              &poolInfo,
                              nullptr,
                              &newHandles->descriptorPool) != VK_SUCCESS)
  {
    throw std::runtime_error("Failed to create descriptor pool.");
  }

  VkDescriptorSetAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocInfo.descriptorPool = newHandles->descriptorPool;
  allocInfo.descriptorSetCount = 1;
  allocInfo.pSetLayouts = &_layoutHandles->commonDescriptorSetLayout;
  if (vkAllocateDescriptorSets( _device.handle(),
                                &allocInfo,
                                &newHandles->descriptorSet) != VK_SUCCESS)
  {
    throw std::runtime_error("Failed to allocate descriptor set.");
  }

  _descriptorHandles = std::move(newHandles);
}

bool PipelineResourceSet::_needUpdateDescriptors() const noexcept
{
  for(const Binding& binding : _staticResources)
  {
    if (binding.lastRevision != binding.resource->revision()) return true;
    if (binding.resource->revision() == 0) Abort("PipelineResourceSet::_needUpdateDescriptors: resource is not initialized.");
  }
  return false;
}

void PipelineResourceSet::scheduleBind(CommandProducer& drawProducer)
{
  if(_layoutHandles == nullptr) _buildLayouts();

  bool needUpdateDescriptors = _needUpdateDescriptors();
  if(needUpdateDescriptors) _descriptorHandles.reset();
  if(_descriptorHandles == nullptr) _buildDescriptors();

  uint32_t setIndex = 0;

  if(_descriptorHandles != nullptr)
  {
    for(size_t iBind = 0; iBind < _staticResources.size(); iBind++)
    {
      Binding& binding = _staticResources[iBind];
      binding.resource->prepareToBind(drawProducer);
      if(needUpdateDescriptors)
      {
        size_t resourceRevision = binding.resource->revision();
        binding.resource->updateDescriptor( _descriptorHandles->descriptorSet,
                                            uint32_t(iBind),
                                            drawProducer,         
                                            _device);
        binding.lastRevision = resourceRevision;
      }
    }
    vkCmdBindDescriptorSets(drawProducer.bufferHandle(),
                            _bindPoint,
                            _layoutHandles->pipelineLayout,
                            setIndex,
                            1,
                            &_descriptorHandles->descriptorSet,
                            0,
                            nullptr);
    setIndex++;
  }

  VkDescriptorSetLayout volatileLayout =
                                    _layoutHandles->volatileDescriptorSetLayout;
  if(volatileLayout != VK_NULL_HANDLE)
  {
    VkDescriptorSet volatileSet = drawProducer.allocateVolatileSet(
                                                    volatileLayout,
                                                    _volatileDescriptorNumber);
    for(size_t iBind = 0; iBind < _volatileResources.size(); iBind++)
    {
      Binding& binding = _volatileResources[iBind];
      binding.resource->prepareToBind(drawProducer);
      binding.resource->updateDescriptor( volatileSet,
                                          uint32_t(iBind),
                                          drawProducer,         
                                          _device);
    }

    vkCmdBindDescriptorSets(drawProducer.bufferHandle(),
                            _bindPoint,
                            _layoutHandles->pipelineLayout,
                            setIndex,
                            1,
                            &volatileSet,
                            0,
                            nullptr);
    setIndex++;
  }

  drawProducer.lockResource(*_layoutHandles);
  drawProducer.lockResource(*_descriptorHandles);
}
