#pragma once

#include <mtt/render/PhysicalDevice.h>

namespace mtt
{
  class RenderLibInstance;

  class RenderSurface
  {
  public:
    RenderSurface(RenderLibInstance& renderLibInstance,
            VkSurfaceKHR& handle) noexcept;
    RenderSurface(const RenderSurface&) = delete;
    RenderSurface& operator = (const RenderSurface&) = delete;
    ~RenderSurface() noexcept;

    inline VkSurfaceKHR handle() const noexcept;

    inline PhysicalDevice::SwapChainSupport device—ompatibility(
                                            const PhysicalDevice& device) const;

  private:
    void _cleanup() noexcept;

  private:
    RenderLibInstance& _renderLibInstance;
    VkSurfaceKHR _handle;
  };

  inline PhysicalDevice::SwapChainSupport RenderSurface::device—ompatibility(
                                            const PhysicalDevice& device) const
  {
    return device.surface—ompatibility(*this);
  }

  inline VkSurfaceKHR RenderSurface::handle() const noexcept
  {
    return _handle;
  }
}