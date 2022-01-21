#pragma once

#include <vulkan/vulkan.h>

#include <mtt/render/Lockable.h>

namespace mtt
{
  class LogicalDevice;

  class Fence : public Lockable
  {
  public:
    explicit Fence(LogicalDevice& device);
    Fence(const Fence&) = delete;
    Fence& operator = (const Fence&) = delete;
  protected:
    virtual ~Fence();

  public:
    inline VkFence handle() const;

    /// timeout in nanoseconds
    void wait(uint64_t timeout = UINT64_MAX);
    void reset();

  private:
    void _clear() noexcept;

  private:
    LogicalDevice& _device;
    VkFence _handle;
  };

  inline VkFence Fence::handle() const
  {
    return _handle;
  }
}