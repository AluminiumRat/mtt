#include <stdexcept>

#include <mtt/render/CommandQueue/QueueFamilyInfo.h>
#include <mtt/render/PhysicalDevice.h>
#include <mtt/render/RenderSurface.h>

using namespace mtt;

QueueFamilyInfo::QueueFamilyInfo( PhysicalDevice& device,
                                  uint32_t familyIndex) :
  _device(&device),
  _index(familyIndex)
{
  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties( _device->handle(),
                                            &queueFamilyCount,
                                            nullptr);

  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties( _device->handle(),
                                            &queueFamilyCount,
                                            queueFamilies.data());
  _properties = queueFamilies[_index];
}

bool QueueFamilyInfo::isPresentSupported(const RenderSurface& surface) const
{
  VkBool32 presentSupport = false;
  if(vkGetPhysicalDeviceSurfaceSupportKHR(_device->handle(),
                                          _index,
                                          surface.handle(),
                                          &presentSupport) != VK_SUCCESS)
  {
    throw std::runtime_error("Unable to get present support.");
  }
  return presentSupport;
}
