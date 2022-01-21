#pragma once

#include <memory>
#include <vector>

#include <mtt/render/CommandQueue/VolatileDescriptorPool.h>

namespace mtt
{
  class LogicalDevice;

  class ScaledVolatileDescriptorPool
  {
  public:
    ScaledVolatileDescriptorPool(
                              const DescriptorCounter& initialDescriptorNumber,
                              uint32_t initialSetNumber,
                              LogicalDevice& device);
    ScaledVolatileDescriptorPool(const ScaledVolatileDescriptorPool&) = delete;
    ScaledVolatileDescriptorPool& operator =
                                  (const ScaledVolatileDescriptorPool&) = delete;
    ~ScaledVolatileDescriptorPool() = default;

    /// Result descriptor set will be returned to the pool in the next frame via
    /// a call of the reset method.
    VkDescriptorSet allocateDescriptorSet(
                                    VkDescriptorSetLayout layout,
                                    const DescriptorCounter& descriptorsNumber);
    void reset();

  private:
    void _addPool();
    void _selectPool(size_t poolIndex);

  private:
    LogicalDevice& _device;
    DescriptorCounter _initialDescriptorNumber;
    uint32_t _initialSetNumber;

    using Subpools = std::vector<std::unique_ptr<VolatileDescriptorPool>>;
    Subpools _subpools;
    size_t _currentPoolIndex;
    VolatileDescriptorPool* _currentPool;
  };
}