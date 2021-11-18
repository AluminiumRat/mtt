#include <stdexcept>

#include <mtt/Render/PhysicalDevice.h>
#include <mtt/Render/RenderSurface.h>

using namespace mtt;

PhysicalDevice::PhysicalDevice( RenderLibInstance& renderLibInstance,
                                VkPhysicalDevice deviceHandle) :
  _renderLibInstance(renderLibInstance),
  _handle(deviceHandle)
{
  vkGetPhysicalDeviceProperties(_handle, &_properties);
  vkGetPhysicalDeviceFeatures(_handle, &_features);

  VkPhysicalDeviceMemoryProperties memoryProperties;
  vkGetPhysicalDeviceMemoryProperties(_handle, &memoryProperties);

  if( memoryProperties.memoryTypeCount == 0 ||
      memoryProperties.memoryHeapCount == 0)
  {
    throw std::runtime_error("Unable to get devices memory properties.");
  }
  
  _memoryInfo.reserve(memoryProperties.memoryTypeCount);
  for(size_t memoryIndex = 0;
      memoryIndex < memoryProperties.memoryTypeCount;
      memoryIndex++)
  {
    MemoryTypeInfo info;
    info.propertyFlags =
                        memoryProperties.memoryTypes[memoryIndex].propertyFlags;
    uint32_t heapIndex = memoryProperties.memoryTypes[memoryIndex].heapIndex;
    info.heapFlags = memoryProperties.memoryHeaps[heapIndex].flags;
    info.heapSize = memoryProperties.memoryHeaps[heapIndex].size;
    _memoryInfo.push_back(info);
  }

  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(_handle, &queueFamilyCount, nullptr);
  _queuesInfo.reserve(queueFamilyCount);
  for(uint32_t queueFamilyIndex = 0;
      queueFamilyIndex < queueFamilyCount;
      queueFamilyIndex++)
  {
    _queuesInfo.emplace_back(*this, queueFamilyIndex);
  }
}

PhysicalDevice::SwapChainSupport PhysicalDevice::surfaceÑompatibility(
                                            const RenderSurface& surface) const
{
  SwapChainSupport support;

  if(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
                                        _handle,
                                        surface.handle(),
                                        &support.capabilities) != VK_SUCCESS)
  {
    throw std::runtime_error("Unable to get surface capabilities.");
  }
    
  uint32_t formatCount;
  if(vkGetPhysicalDeviceSurfaceFormatsKHR( _handle,
                                          surface.handle(),
                                          &formatCount,
                                          nullptr) != VK_SUCCESS)
  {
    throw std::runtime_error("Unable to get surface format count.");
  }

  if (formatCount != 0)
  {
    support.formats.resize(formatCount);
    if(vkGetPhysicalDeviceSurfaceFormatsKHR(
                                        _handle,
                                        surface.handle(),
                                        &formatCount,
                                        support.formats.data()) != VK_SUCCESS)
    {
      throw std::runtime_error("Unable to get surface formats.");
    }
  }

  uint32_t presentModeCount;
  if(vkGetPhysicalDeviceSurfacePresentModesKHR( _handle,
                                                surface.handle(),
                                                &presentModeCount,
                                                nullptr) != VK_SUCCESS)
  {
    throw std::runtime_error("Unable to get surface present mode count.");
  }

  if (presentModeCount != 0)
  {
    support.presentModes.resize(presentModeCount);
    if(vkGetPhysicalDeviceSurfacePresentModesKHR(
                                    _handle,
                                    surface.handle(),
                                    &presentModeCount,
                                    support.presentModes.data()) != VK_SUCCESS)
    {
      throw std::runtime_error("Unable to get surface present modes.");
    }
  }

  return support;
}
