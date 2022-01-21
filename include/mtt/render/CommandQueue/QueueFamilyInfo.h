#pragma once

#include <vulkan/vulkan.h>

namespace mtt
{
  class PhysicalDevice;
  class RenderSurface;

  class QueueFamilyInfo
  {
  public:
    QueueFamilyInfo(PhysicalDevice& device, uint32_t familyIndex);
    QueueFamilyInfo(const QueueFamilyInfo&) = default;
    QueueFamilyInfo& operator = (const QueueFamilyInfo&) = default;
    ~QueueFamilyInfo() noexcept = default;

    inline uint32_t index() const noexcept;
    inline PhysicalDevice& device() const noexcept;

    inline VkQueueFlags queueFlags() const noexcept;
    inline uint32_t queueCount() const noexcept;
    inline uint32_t timestampValidBits() const noexcept;
    inline VkExtent3D minImageTransferGranularity() const noexcept;

    bool isPresentSupported(const RenderSurface& surface) const;

  private:
    PhysicalDevice* _device;
    uint32_t _index;

    VkQueueFamilyProperties _properties;
  };

  inline uint32_t QueueFamilyInfo::index() const noexcept
  {
    return _index;
  }

  inline PhysicalDevice& QueueFamilyInfo::device() const noexcept
  {
    return *_device;
  }

  inline VkQueueFlags QueueFamilyInfo::queueFlags() const noexcept
  {
    return _properties.queueFlags;
  }

  inline uint32_t QueueFamilyInfo::queueCount() const noexcept
  {
    return _properties.queueCount;
  }

  inline uint32_t QueueFamilyInfo::timestampValidBits() const noexcept
  {
    return _properties.timestampValidBits;
  }

  inline VkExtent3D
                  QueueFamilyInfo::minImageTransferGranularity() const noexcept
  {
    return _properties.minImageTransferGranularity;
  }
}