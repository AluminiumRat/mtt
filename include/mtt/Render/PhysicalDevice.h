#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include <mtt/Render/CommandQueue/QueueFamilyInfo.h>

namespace mtt
{
  class RenderLibInstance;
  class RenderSurface;

  class PhysicalDevice
  {
  public:
    struct MemoryTypeInfo
    {
      VkMemoryPropertyFlags propertyFlags;
      VkMemoryHeapFlags heapFlags;
      VkDeviceSize heapSize;
    };
    using MemoryInfo = std::vector<MemoryTypeInfo>;

    struct SwapChainSupport
    {
      VkSurfaceCapabilitiesKHR capabilities;
      std::vector<VkSurfaceFormatKHR> formats;
      std::vector<VkPresentModeKHR> presentModes;
    };

    using QueuesInfo = std::vector<QueueFamilyInfo>;

  public:
    PhysicalDevice( RenderLibInstance& renderLibInstance,
                    VkPhysicalDevice deviceHandle);
    PhysicalDevice(const PhysicalDevice&) = delete;
    PhysicalDevice& operator = (const PhysicalDevice&) = delete;
    virtual ~PhysicalDevice() noexcept = default;

    inline VkPhysicalDevice handle() const noexcept;

    inline RenderLibInstance& renderInstance() const noexcept;

    inline const VkPhysicalDeviceProperties& properties() const noexcept;
    inline const VkPhysicalDeviceFeatures& features() const noexcept;
    inline MemoryInfo memoryInfo() const noexcept;
    inline QueuesInfo queuesInfo() const noexcept;

    SwapChainSupport surface—ompatibility(const RenderSurface& surface) const;
    inline bool isSurfaceSuitable(const RenderSurface& surface) const;

  private:
    RenderLibInstance& _renderLibInstance;
    VkPhysicalDevice _handle;

    VkPhysicalDeviceProperties _properties;
    VkPhysicalDeviceFeatures _features;
    MemoryInfo _memoryInfo;
    QueuesInfo _queuesInfo;
  };

  inline VkPhysicalDevice PhysicalDevice::handle() const noexcept
  {
    return _handle;
  }

  inline RenderLibInstance& PhysicalDevice::renderInstance() const noexcept
  {
    return _renderLibInstance;
  }

  inline const VkPhysicalDeviceProperties&
                                    PhysicalDevice::properties() const noexcept
  {
    return _properties;
  }

  inline const VkPhysicalDeviceFeatures&
                                      PhysicalDevice::features() const noexcept
  {
    return _features;
  }

  inline PhysicalDevice::MemoryInfo PhysicalDevice::memoryInfo() const noexcept
  {
    return _memoryInfo;
  }

  inline PhysicalDevice::QueuesInfo PhysicalDevice::queuesInfo() const noexcept
  {
    return _queuesInfo;
  }

  inline bool PhysicalDevice::isSurfaceSuitable(
                                            const RenderSurface& surface) const
  {
    SwapChainSupport support = surface—ompatibility(surface);
    return support.formats.size() != 0 && support.presentModes.size() != 0;
  }
}