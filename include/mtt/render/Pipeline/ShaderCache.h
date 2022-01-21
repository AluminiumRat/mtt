#pragma once

#include <string>
#include <map>
#include <mutex>

#include <vulkan/vulkan.h>

#include <mtt/render/Pipeline/ShaderModule.h>

namespace mtt
{
  class ShaderCache
  {
  public:
    class ModuleObject
    {
    public:
      inline ModuleObject() noexcept;
      inline ModuleObject(LogicalDevice& device,
                          VkShaderModule handle) noexcept;
      ModuleObject(const ModuleObject&) = delete;
      inline ModuleObject(ModuleObject&& other) noexcept;
      ModuleObject& operator = (const ModuleObject&) = delete;
      inline ModuleObject& operator = (ModuleObject&& other) noexcept;
      inline ~ModuleObject() noexcept;

      inline VkShaderModule handle() const noexcept;

    private:
      void release() noexcept;
    
    private:
      LogicalDevice* _device;
      VkShaderModule _handle;
    };

  public:
    ShaderCache() = default;
    ShaderCache(const ShaderCache&) = delete;
    ShaderCache& operator = (const ShaderCache&) = delete;
    ~ShaderCache() noexcept = default;
  
    VkShaderModule find(ShaderModule::Type type,
                        const std::string& searchString) const noexcept;

    /// Returns the handle of saved module. If the module already exists in
    /// the cache, the new module object will be destroyed and the old handle
    /// returned.
    VkShaderModule add( ShaderModule::Type type,
                        const std::string& searchString,
                        ModuleObject&& moduleObject);
  
    void clear() noexcept;
  
  private:
    struct ModuleIndex
    {
      ShaderModule::Type type;
      std::string searchString;
    
      inline bool operator == (const ModuleIndex& right) const noexcept;
      inline bool operator < (const ModuleIndex& right) const noexcept;
    };

  private:
    using ModuleMap = std::map<ModuleIndex, ModuleObject>;
    ModuleMap _moduleMap;
    mutable std::mutex _mapMutex;
  };

  inline ShaderCache::ModuleObject::ModuleObject() noexcept :
    _device(nullptr),
    _handle(VK_NULL_HANDLE)
  {
  }

  inline ShaderCache::ModuleObject::ModuleObject(
                                              LogicalDevice& device,
                                              VkShaderModule handle) noexcept :
    _device(&device),
    _handle(handle)
  {
  }

  inline ShaderCache::ModuleObject::ModuleObject(
                                  ShaderCache::ModuleObject&& other) noexcept :
    _device(other._device),
    _handle(other._handle)
  {
    other._handle = VK_NULL_HANDLE;
  }

  inline ShaderCache::ModuleObject& ShaderCache::ModuleObject::operator = (
                                    ShaderCache::ModuleObject&& other) noexcept
  {
    release();
    _device = other._device;
    _handle = other._handle;
    other._handle = VK_NULL_HANDLE;
    return *this;
  }

  inline ShaderCache::ModuleObject::~ModuleObject() noexcept
  {
    release();
  }

  inline VkShaderModule ShaderCache::ModuleObject::handle() const noexcept
  {
    return _handle;
  }

  inline bool ShaderCache::ModuleIndex::operator == (
                          const ShaderCache::ModuleIndex& right) const noexcept
  {
    return type == right.type && searchString == right.searchString;
  }

  inline bool ShaderCache::ModuleIndex::operator < (
                          const ShaderCache::ModuleIndex& right) const noexcept
  {
    if(type < right.type) return true;
    if(type > right.type) return false;
    if(searchString < right.searchString) return true;
    return false;
  }
}