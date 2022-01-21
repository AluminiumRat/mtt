#pragma once

#include <vulkan/vulkan.h>

#include <mtt/render/Utilities/DescriptorCounter.h>

namespace mtt
{
  class LogicalDevice;

  class VolatileDescriptorPool
  {
  public:
    VolatileDescriptorPool( const DescriptorCounter& totalDescriptors,
                            uint32_t totalSets,
                            LogicalDevice& device);
    VolatileDescriptorPool(const VolatileDescriptorPool&) = delete;
    VolatileDescriptorPool& operator = (const VolatileDescriptorPool&) = delete;
    ~VolatileDescriptorPool();

    inline VkDescriptorPool handle() const;
    inline LogicalDevice& device() const;

    inline const DescriptorCounter& totalDescriptors() const;
    inline const DescriptorCounter& descriptorsLeft() const;

    inline uint32_t totalSets() const;
    inline uint32_t setsLeft() const;
    inline uint32_t setsAllocated() const;

    /// Result descriptor set will be returned to the pool in the next frame via
    /// a call of the reset method.
    VkDescriptorSet allocateDescriptorSet(
                                    VkDescriptorSetLayout layout,
                                    const DescriptorCounter& descriptorsNumber);
    void reset();

  private:
    void _cleanup() noexcept;

  private:
    VkDescriptorPool _handle;
    LogicalDevice& _device;

    DescriptorCounter _totalDescriptors;
    DescriptorCounter _descriptorsLeft;

    uint32_t _totalSets;
    uint32_t _setsLeft;
  };

  inline VkDescriptorPool VolatileDescriptorPool::handle() const
  {
    return _handle;
  }

  inline LogicalDevice& VolatileDescriptorPool::device() const
  {
    return _device;
  }

  inline const DescriptorCounter&
                                VolatileDescriptorPool::totalDescriptors() const
  {
    return _totalDescriptors;
  }

  inline const DescriptorCounter&
                                VolatileDescriptorPool::descriptorsLeft() const
  {
    return _descriptorsLeft;
  }

  inline uint32_t VolatileDescriptorPool::totalSets() const
  {
    return _totalSets;
  }

  inline uint32_t VolatileDescriptorPool::setsLeft() const
  {
    return _setsLeft;
  }

  inline uint32_t VolatileDescriptorPool::setsAllocated() const
  {
    return _totalSets - _setsLeft;
  }
}