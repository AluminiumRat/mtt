#include <array>
#include <stdexcept>

#include <mtt/render/LogicalDevice.h>
#include <mtt/render/PhysicalDevice.h>
#include <mtt/render/RenderLibInstance.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

const char* const swapchainExtensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
const char* const validationLayers[] = { "VK_LAYER_KHRONOS_validation" };

const size_t DRAW_POOL_SIZE = 5;
const size_t TRANSFER_POOL_SIZE = 2;

static int bitCount(unsigned int sample)
{
  int counter = 0;
  while(sample != 0)
  {
    if(sample & 1) counter++;
    sample /= 2;
  }
  return counter;
}

LogicalDevice::LogicalDevice( PhysicalDevice& physicalDevice,
                              const VkPhysicalDeviceFeatures& features,
                              RenderSurface* presentationTargetExample) :
  _physicalDevice(physicalDevice),
  _handle(VK_NULL_HANDLE),
  _allocator(VK_NULL_HANDLE),
  _features(features),
  _drawFamilyIndex(0),
  _drawQueueIndex(0),
  _drawQueue(nullptr),
  _presentationFamilyIndex(0),
  _presentationQueueIndex(0),
  _presentationQueue(nullptr),
  _transferFamilyIndex(0),
  _transferQueueIndex(0),
  _transferQueue(nullptr)
{
  try
  {
    _searchFamilies(presentationTargetExample);

    PhysicalDevice::QueuesInfo queuesInfo = _physicalDevice.queuesInfo();
    std::vector<uint32_t> queueCounters(queuesInfo.size(), 0);
    queueCounters[_drawFamilyIndex]++;
    if(_presentationFamilyIndex != _drawFamilyIndex)
    {
      queueCounters[_presentationFamilyIndex]++;
    }
    queueCounters[_transferFamilyIndex]++;

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::array<float, 3> priorities = { 1.f, 1.f, 1.f };
    for(uint32_t familyIndex = 0;
        familyIndex < queueCounters.size();
        familyIndex++)
    {
      if(queueCounters[familyIndex] != 0)
      {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = familyIndex;
        queueCreateInfo.queueCount = queueCounters[familyIndex];
        queueCreateInfo.pQueuePriorities = priorities.data();
        queueCreateInfos.push_back(queueCreateInfo);
      }
    }

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = uint32_t(queueCreateInfos.size());
    createInfo.pEnabledFeatures = &features;

    if(presentationTargetExample != nullptr)
    {
      createInfo.enabledExtensionCount = 1;
      createInfo.ppEnabledExtensionNames = swapchainExtensions;
    }
    else
    {
      createInfo.enabledExtensionCount = 0;
      createInfo.ppEnabledExtensionNames = nullptr;
    }

    if (_physicalDevice.renderInstance().isValidationEnabled())
    {
      createInfo.enabledLayerCount = 1;
      createInfo.ppEnabledLayerNames = validationLayers;
    }
    else
    {
      createInfo.enabledLayerCount = 0;
      createInfo.ppEnabledLayerNames = nullptr;
    }
  
    if (vkCreateDevice( _physicalDevice.handle(),
                        &createInfo,
                        nullptr,
                        &_handle) != VK_SUCCESS)
    {
      throw std::runtime_error("LogicalDevice: Failed to create logical device.");
    }

    VmaAllocatorCreateInfo allocatorInfo = {};
    allocatorInfo.vulkanApiVersion =
                            _physicalDevice.renderInstance().vulkanApiVersion();
    allocatorInfo.physicalDevice = physicalDevice.handle();
    allocatorInfo.device = _handle;
    allocatorInfo.instance = _physicalDevice.renderInstance().handle();
    if(vmaCreateAllocator(&allocatorInfo, &_allocator) != VK_SUCCESS)
    {
      throw std::runtime_error("LogicalDevice: Failed to create memory allocator.");
    }

    _buildQueues(presentationTargetExample != nullptr);
  }
  catch(...)
  {
    _cleanup();
    throw;
  }
}

void LogicalDevice::_searchFamilies(RenderSurface* presentationTargetExample)
{
  bool needPresentation = presentationTargetExample != nullptr;

  PhysicalDevice::QueuesInfo queuesInfo = _physicalDevice.queuesInfo();
  _drawFamilyIndex = queuesInfo.size();
  _transferFamilyIndex = queuesInfo.size();
  int transferFamilyBits = 256;
  _presentationFamilyIndex = queuesInfo.size();

  for(size_t familyIndex = 0;
      familyIndex < queuesInfo.size();
      familyIndex++)
  {
    VkQueueFlags flags = queuesInfo[familyIndex].queueFlags();
    if( (flags & VK_QUEUE_GRAPHICS_BIT) &&
        (flags & VK_QUEUE_COMPUTE_BIT) &&
        (flags & VK_QUEUE_TRANSFER_BIT) &&
        _drawFamilyIndex == queuesInfo.size())
    {
      _drawFamilyIndex = familyIndex;
    }

    if ((flags & VK_QUEUE_TRANSFER_BIT) &&
        (flags & VK_QUEUE_GRAPHICS_BIT) &&
        bitCount(flags) < transferFamilyBits)
    {
      _transferFamilyIndex = familyIndex;
      transferFamilyBits = bitCount(flags);
    }

    if( needPresentation &&
        _presentationFamilyIndex == queuesInfo.size() &&
        queuesInfo[familyIndex].isPresentSupported(
                                                  *presentationTargetExample))
    {
      _presentationFamilyIndex = familyIndex;
    }
  }

  if (_drawFamilyIndex == queuesInfo.size()) throw std::runtime_error("LogicalDevice: No suitable draw queue found");
  if (needPresentation && _presentationFamilyIndex == queuesInfo.size())
  {
    throw std::runtime_error("LogicalDevice: No suitable presentation queue found");
  }
  if (_transferFamilyIndex == queuesInfo.size()) throw std::runtime_error("LogicalDevice: No suitable transfer queue found");
}

void LogicalDevice::_buildQueues(bool presentationEnabled)
{
  _drawQueueIndex = 0;
  std::unique_ptr<CommandQueue> newQueue(new CommandQueue(
                                                  *this,
                                                  uint32_t(_drawFamilyIndex),
                                                  _drawQueueIndex,
                                                  DRAW_POOL_SIZE));
  _drawQueue = newQueue.get();
  _queues.push_back(std::move(newQueue));

  _transferQueueIndex = 0;
  if(_transferFamilyIndex == _drawFamilyIndex) _transferQueueIndex++;
  newQueue.reset(new CommandQueue(*this,
                                  uint32_t(_transferFamilyIndex),
                                  _transferQueueIndex,
                                  TRANSFER_POOL_SIZE));
  _transferQueue = newQueue.get();
  _queues.push_back(std::move(newQueue));

  if(presentationEnabled)
  {
    if(_presentationFamilyIndex == _drawFamilyIndex)
    {
      _presentationQueueIndex = _drawQueueIndex;
      _presentationQueue = _drawQueue;
    }
    else
    {
      _presentationQueueIndex = 0;
      if(_presentationFamilyIndex == _transferFamilyIndex)
      {
        _presentationQueueIndex++;
      }
      newQueue.reset(new CommandQueue(*this,
                                      uint32_t(_presentationFamilyIndex),
                                      _presentationQueueIndex,
                                      DRAW_POOL_SIZE));
      _presentationQueue = newQueue.get();
      _queues.push_back(std::move(newQueue));
    }
  }
  else _presentationQueue = nullptr;
}

LogicalDevice::~LogicalDevice()
{
  _cleanup();
}

void LogicalDevice::_cleanup() noexcept
{
  _shaderCache.clear();
  _drawQueue = nullptr;
  _presentationQueue = nullptr;
  _transferQueue = nullptr;
  _queues.clear();

  if (_allocator != VK_NULL_HANDLE)
  {
    vmaDestroyAllocator(_allocator);
    _allocator = VK_NULL_HANDLE;
  }

  if (_handle != VK_NULL_HANDLE)
  {
    vkDeviceWaitIdle(_handle);
    vkDestroyDevice(_handle, nullptr);
    _handle = VK_NULL_HANDLE;
  }
}

bool LogicalDevice::isSurfaceSuitable(const RenderSurface& surface) const
{
  if(!_physicalDevice.isSurfaceSuitable(surface)) return false;

  PhysicalDevice::QueuesInfo queuesInfo = _physicalDevice.queuesInfo();
  if(_presentationFamilyIndex == queuesInfo.size()) return false;

  return queuesInfo[_presentationFamilyIndex].isPresentSupported(surface);
}
