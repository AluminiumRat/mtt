#include <stdexcept>

#include <mtt/render/CommandQueue/Fence.h>
#include <mtt/render/LogicalDevice.h>

using namespace mtt;

Fence::Fence(LogicalDevice& device) :
  _device(device),
  _handle(VK_NULL_HANDLE)
{
  try
  {
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    if (vkCreateFence(_device.handle(),
                      &fenceInfo,
                      nullptr,
                      &_handle) != VK_SUCCESS)
    {
      throw std::runtime_error("Failed to create fence.");
    }
  }
  catch(...)
  {
    _clear();
    throw;
  }
}

void Fence::wait(uint64_t timeout)
{
  if (_handle == VK_NULL_HANDLE) return;

  if(vkWaitForFences( _device.handle(),
                      1,
                      &_handle,
                      VK_TRUE,
                      timeout) != VK_SUCCESS)
  {
    throw std::runtime_error("Unable to wait fence.");
  }
}

void Fence::reset()
{
  if (vkResetFences(_device.handle(), 1, &_handle) != VK_SUCCESS)
  {
    throw std::runtime_error("Unable to reset fence.");
  }
}

Fence::~Fence()
{
  _clear();
}

void Fence::_clear() noexcept
{
  if(_handle != VK_NULL_HANDLE)
  {
    vkDestroyFence(_device.handle(), _handle, nullptr);
    _handle = VK_NULL_HANDLE;
  }
}
