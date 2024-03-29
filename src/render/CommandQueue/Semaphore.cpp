#include <stdexcept>

#include <mtt/render/CommandQueue/Semaphore.h>
#include <mtt/render/LogicalDevice.h>

using namespace mtt;

Semaphore::Semaphore(LogicalDevice& device) :
  _device(device),
  _handle(VK_NULL_HANDLE)
{
  try
  {
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    if (vkCreateSemaphore(_device.handle(),
                          &semaphoreInfo,
                          nullptr,
                          &_handle) != VK_SUCCESS)
    {
      throw std::runtime_error("Semaphore: Failed to create semaphore.");
    }
  }
  catch(...)
  {
    _clear();
    throw;
  }
}

Semaphore::~Semaphore()
{
  _clear();
}

void Semaphore::_clear() noexcept
{
  if(_handle != VK_NULL_HANDLE)
  {
    vkDestroySemaphore(_device.handle(), _handle, nullptr);
    _handle = VK_NULL_HANDLE;
  }
}
