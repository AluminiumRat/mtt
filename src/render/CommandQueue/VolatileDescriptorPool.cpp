#include <stdexcept>

#include <mtt/render/CommandQueue/VolatileDescriptorPool.h>
#include <mtt/render/LogicalDevice.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

VolatileDescriptorPool::VolatileDescriptorPool(
                                    const DescriptorCounter& totalDescriptors,
                                    uint32_t totalSets,
                                    LogicalDevice& device) :
  _handle(VK_NULL_HANDLE),
  _device(device),
  _totalDescriptors(totalDescriptors),
  _descriptorsLeft(_totalDescriptors),
  _totalSets(totalSets),
  _setsLeft(totalSets)
{
  try
  {
    std::vector<VkDescriptorPoolSize> sizeInfo =
                                              _totalDescriptors.makeSizeInfo();
    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = uint32_t(sizeInfo.size());
    poolInfo.pPoolSizes = sizeInfo.data();
    poolInfo.maxSets = _totalSets;
    if(vkCreateDescriptorPool(_device.handle(),
                              &poolInfo,
                              nullptr,
                              &_handle) != VK_SUCCESS)
    {
      throw std::runtime_error("VolatileDescriptorPool: Failed to create descriptor pool.");
    }
  }
  catch(...)
  {
    _cleanup();
    throw;
  }
}

VolatileDescriptorPool::~VolatileDescriptorPool()
{
  _cleanup();
}

void VolatileDescriptorPool::_cleanup() noexcept
{
  if(_handle != VK_NULL_HANDLE)
  {
    vkDestroyDescriptorPool(_device.handle(), _handle, nullptr);
    _handle = VK_NULL_HANDLE;
  }
}

VkDescriptorSet VolatileDescriptorPool::allocateDescriptorSet(
                                    VkDescriptorSetLayout layout,
                                    const DescriptorCounter& descriptorsNumber)
{
  if(!_descriptorsLeft.contains(descriptorsNumber)) Abort("VolatileDescriptorPool::allocateDescriptorSet: too few descriptors left.");
  if (_setsLeft == 0) Abort("VolatileDescriptorPool::allocateDescriptorSet: descriptors pool is empty.");

  VkDescriptorSetAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocInfo.descriptorPool = _handle;
  allocInfo.descriptorSetCount = 1;
  allocInfo.pSetLayouts = &layout;
  VkDescriptorSet newSet = VK_NULL_HANDLE;
  if(vkAllocateDescriptorSets(_device.handle(),
                              &allocInfo,
                              &newSet) != VK_SUCCESS)
  {
    throw std::runtime_error("VolatileDescriptorPool: Failed to allocate uniform descriptor set.");
  }

  _descriptorsLeft.reduce(descriptorsNumber);
  _setsLeft--;

  return newSet;
}

void VolatileDescriptorPool::reset()
{
  if(vkResetDescriptorPool(_device.handle(), _handle, 0) != VK_SUCCESS)
  {
    throw std::runtime_error("VolatileDescriptorPool: Failed to reset volatile descriptor pool.");
  }
  _descriptorsLeft = _totalDescriptors;
  _setsLeft = _totalSets;
}
