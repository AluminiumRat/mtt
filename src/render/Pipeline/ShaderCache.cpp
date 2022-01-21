#include <mtt/render/Pipeline/ShaderCache.h>
#include <mtt/render/LogicalDevice.h>

using namespace mtt;

void ShaderCache::ModuleObject::release() noexcept
{
  if (_handle != VK_NULL_HANDLE)
  {
    vkDestroyShaderModule(_device->handle(), _handle, nullptr);
    _handle = VK_NULL_HANDLE;
  }
}

VkShaderModule ShaderCache::find(
                                ShaderModule::Type type,
                                const std::string& searchString) const noexcept
{
  ModuleIndex index{type, searchString};
  std::lock_guard<std::mutex> lock(_mapMutex);
  ModuleMap::const_iterator iModule = _moduleMap.find(index);
  if(iModule != _moduleMap.end()) return iModule->second.handle();
  return VK_NULL_HANDLE;
}

VkShaderModule ShaderCache::add(ShaderModule::Type type,
                                const std::string& searchString,
                                ModuleObject&& moduleObject)
{
  ModuleIndex index{type, searchString};
  std::lock_guard<std::mutex> lock(_mapMutex);
  ModuleMap::const_iterator iModule = _moduleMap.find(index);
  if (iModule != _moduleMap.end()) iModule->second.handle();
  VkShaderModule handle = moduleObject.handle();
  _moduleMap[index] = std::move(moduleObject);
  return handle;
}

void ShaderCache::clear() noexcept
{
  std::lock_guard<std::mutex> lock(_mapMutex);
  _moduleMap.clear();
}
