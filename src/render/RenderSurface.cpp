#include <stdexcept>

#include <mtt/render/RenderLibInstance.h>
#include <mtt/render/RenderSurface.h>

using namespace mtt;

RenderSurface::RenderSurface( RenderLibInstance& renderLibInstance,
                  VkSurfaceKHR& handle) noexcept :
  _renderLibInstance(renderLibInstance),
  _handle(handle)
{
}

RenderSurface::~RenderSurface() noexcept
{
  _cleanup();
}

void RenderSurface::_cleanup() noexcept
{
  if (_handle != VK_NULL_HANDLE)
  {
    vkDestroySurfaceKHR(_renderLibInstance.handle(), _handle, nullptr);
    _handle = VK_NULL_HANDLE;
  }
}
