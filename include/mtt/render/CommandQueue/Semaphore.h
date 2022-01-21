#pragma once

#include <vulkan/vulkan.h>

#include <mtt/render/Lockable.h>

namespace mtt
{
  class LogicalDevice;

  class Semaphore : public Lockable
  {
  public:
    explicit Semaphore(LogicalDevice& device);
    Semaphore(const Semaphore&) = delete;
    Semaphore& operator = (const Semaphore&) = delete;
  protected:
    virtual ~Semaphore();

  public:
    inline VkSemaphore handle() const;

  private:
    void _clear() noexcept;

  private:
    LogicalDevice& _device;
    VkSemaphore _handle;
  };

  inline VkSemaphore Semaphore::handle() const
  {
    return _handle;
  }
}