#pragma once

#include <mtt/render/PhysicalDevice.h>

namespace mtt
{
  class RenderSurface
  {
  public:
    RenderSurface(VkSurfaceKHR& handle) noexcept;
    RenderSurface(const RenderSurface&) = delete;
    RenderSurface& operator = (const RenderSurface&) = delete;
    ~RenderSurface() noexcept;

    inline VkSurfaceKHR handle() const noexcept;

    inline PhysicalDevice::SwapChainSupport deviceÑompatibility(
                                            const PhysicalDevice& device) const;

  private:
    void _cleanup() noexcept;

  private:
    VkSurfaceKHR _handle;
  };

  inline PhysicalDevice::SwapChainSupport RenderSurface::deviceÑompatibility(
                                            const PhysicalDevice& device) const
  {
    return device.surfaceÑompatibility(*this);
  }

  inline VkSurfaceKHR RenderSurface::handle() const noexcept
  {
    return _handle;
  }
}