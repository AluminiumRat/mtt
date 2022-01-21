#include <mtt/render/CommandQueue/CommandProducer.h>
#include <mtt/render/Pipeline/AbstractPipeline.h>
#include <mtt/render/LogicalDevice.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

AbstractPipeline::Handle::~Handle() noexcept
{
  if(value != VK_NULL_HANDLE)
  {
    if(device == VK_NULL_HANDLE) Abort("AbstractPipeline::Handle::~Handle: diveci is null.");
    vkDestroyPipeline(device, value, nullptr);
    value = VK_NULL_HANDLE;
    device = VK_NULL_HANDLE;
  }
}

AbstractPipeline::AbstractPipeline( VkPipelineBindPoint bindPoint,
                                    LogicalDevice& device) :
  _device(device),
  _bindPoint(bindPoint),
  _resourceSet(*this, bindPoint, device)
{
}

void AbstractPipeline::addShader(std::unique_ptr<ShaderModule> shaderModule)
{
  if(shaderModule == nullptr) Abort("AbstractPipeline::addShader: shader module is null.");

  ShaderRecord newRecord;
  switch (shaderModule->type())
  {
    case ShaderModule::VERTEX_SHADER:
      newRecord.stage = VK_SHADER_STAGE_VERTEX_BIT;
      break;
    case ShaderModule::FRAGMENT_SHADER:
      newRecord.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
      break;
    case ShaderModule::COMPUTE_SHADER:
      newRecord.stage = VK_SHADER_STAGE_COMPUTE_BIT;
      break;
    case ShaderModule::GEOMETRY_SHADER:
      newRecord.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
      break;
    case ShaderModule::TESSELLATION_CONTROL_SHADER:
      newRecord.stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
      break;
    case ShaderModule::TESSELLATION_EVALUATION_SHADER:
      newRecord.stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
      break;
  }

  for(size_t iBinding = 0;
      iBinding < _resourceSet.resourceNumber();
      iBinding++)
  {
    const PipelineResourceSet::Binding& binding =
                                            _resourceSet.resource(iBinding);
    shaderModule->setDefine(binding.bindingName);
  }

  for(Defines::iterator iDefine = _defines.begin();
      iDefine != _defines.end();
      iDefine++)
  {
    shaderModule->setDefine(iDefine->first, iDefine->second);
  }

  newRecord.shaderModule.swap(shaderModule);
  _shaders.push_back(std::move(newRecord));
  invalidate();
}

std::unique_ptr<ShaderModule> AbstractPipeline::removeShader(
                                          ShaderModule& shaderModule) noexcept
{
  for(Shaders::iterator iShader = _shaders.begin();
      iShader != _shaders.end();
      iShader++)
  {
    if(iShader->shaderModule.get() == &shaderModule)
    {
      std::unique_ptr<ShaderModule> result = std::move(iShader->shaderModule);
      _shaders.erase(iShader);
      invalidate();
      return result;
    }
  }
  Abort("AbstractPipeline::removeShader: the shader module is not part of this pipeline.");
}

void AbstractPipeline::setDefine( const std::string& name,
                                  const std::string& value)
{
  _defines[name] = value;
  for (const ShaderRecord& shader : _shaders)
  {
    shader.shaderModule->setDefine(name, value);
  }
}

void AbstractPipeline::setDefine(const std::string& name)
{
  setDefine(name, "");
}

void AbstractPipeline::removeDefine(const std::string& name) noexcept
{
  try
  {
    _defines.erase(name);
  }
  catch(...)
  {
    Abort("AbstractPipeline::removeDefine: unable to remove define from define map.");
  }

  for (const ShaderRecord& shader : _shaders)
  {
    shader.shaderModule->removeDefine(name);
  }
}

bool AbstractPipeline::_shadersChanged() const noexcept
{
  for (const ShaderRecord& shader : _shaders)
  {
    if(shader.shaderModule->revision() != shader.lastRevision) return true;
  }
  return false;
}

void AbstractPipeline::scheduleBind(CommandProducer& drawProducer)
{
  if(_handle == nullptr || _shadersChanged()) rebuild();
  vkCmdBindPipeline(drawProducer.bufferHandle(), _bindPoint, _handle->value);
  
  _resourceSet.scheduleBind(drawProducer);
  
  drawProducer.lockResource(*_handle);
}

void AbstractPipeline::forceBuild()
{
  rebuild();
}

void AbstractPipeline::rebuild()
{
  _handle.reset();

  _adjustDescriptorSets();
  _adjustResourceLocations();

  Ref<Handle> newHandle(new Handle);
  newHandle->device = _device.handle();

  if(_shaders.empty()) Abort("GraphicsPipeline::_rebuild: no shaders.");
  std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
  shaderStages.reserve(_shaders.size());
  for(ShaderRecord& shader : _shaders)
  {
    VkPipelineShaderStageCreateInfo shaderInfo{};
    shaderInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderInfo.stage = shader.stage;
    shaderInfo.module = shader.shaderModule->handle();
    if (shaderInfo.module == VK_NULL_HANDLE) Abort("GraphicsPipeline::_rebuild: shader module is not valid.");
    shaderInfo.pName = "main";
    shaderStages.push_back(shaderInfo);

    shader.lastRevision = shader.shaderModule->revision();
  }

  newHandle->value = createPipeline(shaderStages,
                                    _resourceSet.pipelineLayoutHandle());

  _handle = std::move(newHandle);
}

void AbstractPipeline::_adjustDescriptorSets()
{
  std::string staticSet = "0";
  std::string volatileSet = _resourceSet.staticResourceNumber() == 0 ?  "0" :
                                                                        "1";

  for (ShaderRecord& shader : _shaders)
  {
    shader.shaderModule->setDefine("staticSet", staticSet);
    shader.shaderModule->setDefine("volatileSet", volatileSet);
  }
}

void AbstractPipeline::_adjustResourceLocations()
{
  for(size_t iBinding = 0;
      iBinding < _resourceSet.staticResourceNumber();
      iBinding++)
  {
    std::string strIndex = std::to_string(iBinding);
    const PipelineResourceSet::Binding& binding =
                                          _resourceSet.staticResource(iBinding);
    for (ShaderRecord& shader : _shaders)
    {
      shader.shaderModule->setDefine(binding.bindingName, strIndex);
    }
  }

  for(size_t iBinding = 0;
      iBinding < _resourceSet.volatileResourceNumber();
      iBinding++)
  {
    std::string strIndex = std::to_string(iBinding);

    const PipelineResourceSet::Binding& binding =
                                        _resourceSet.volatileResource(iBinding);
    for (ShaderRecord& shader : _shaders)
    {
      shader.shaderModule->setDefine(binding.bindingName, strIndex);
    }
  }
}

void AbstractPipeline::addResource( const std::string& bindingName,
                                    PipelineResource& resource,
                                    VkShaderStageFlags stageFlags)
{
  _resourceSet.addResource(resource, stageFlags, bindingName);
  for (ShaderRecord& shader : _shaders)
  {
    shader.shaderModule->setDefine(bindingName);
  }
}

void AbstractPipeline::removeResource(const std::string& bindingName) noexcept
{
  for(size_t iBinding = 0;
      iBinding < _resourceSet.resourceNumber();
      iBinding++)
  {
    const PipelineResourceSet::Binding& binding =
                                                _resourceSet.resource(iBinding);
    if(binding.bindingName == bindingName)
    {
      _resourceSet.removeResource(iBinding);

      for (ShaderRecord& shader : _shaders)
      {
        shader.shaderModule->removeDefine(bindingName);
      }

      break;
    }
  }

  Abort("GraphicsPipeline::removeResource: resource is not found.");
}
