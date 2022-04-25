#include <stdexcept>

#include <mtt/render/LogicalDevice.h>
#include <mtt/render/PhysicalDevice.h>
#include <mtt/render/RenderSurface.h>
#include <mtt/render/SwapChain.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

VkSurfaceFormatKHR chooseSwapSurfaceFormat(
                        const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
  for (const VkSurfaceFormatKHR& availableFormat : availableFormats)
  {
    if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
        availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
    {
      return availableFormat;
    }
  }

  return availableFormats[0];
}

static VkPresentModeKHR chooseSwapPresentMode(
                    const std::vector<VkPresentModeKHR>& availablePresentModes)
{
  for (const VkPresentModeKHR& availablePresentMode : availablePresentModes)
  {
    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
    {
      return availablePresentMode;
    }
  }
  return VK_PRESENT_MODE_FIFO_KHR;
}

static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
  if (capabilities.currentExtent.width != UINT32_MAX)
  {
    return capabilities.currentExtent;
  }
  else
  {
    VkExtent2D actualExtent = { 800, 600 };

    actualExtent.width = std::max(capabilities.minImageExtent.width,
                                  std::min( capabilities.maxImageExtent.width,
                                            actualExtent.width));
    actualExtent.height = std::max(
                                capabilities.minImageExtent.height,
                                std::min( capabilities.maxImageExtent.height,
                                          actualExtent.height));
    return actualExtent;
  }
}

static uint32_t chooseImageCount(const VkSurfaceCapabilitiesKHR& capabilities)
{
  uint32_t imageCount = capabilities.minImageCount + 1;
  if (capabilities.maxImageCount > 0 &&
      imageCount > capabilities.maxImageCount)
  {
    imageCount = capabilities.maxImageCount;
  }
  return imageCount;
}

static std::vector<uint32_t> getFamilyIndices(
                                  const std::vector<CommandQueue const*>& queues)
{
  std::vector<uint32_t> indices;
  for(const CommandQueue* queue : queues)
  {
    uint32_t newIndex = queue->familyInfo().index();
    bool needInsert = true;;
    for(uint32_t existingIndex : indices)
    {
      if(newIndex == existingIndex)
      {
        needInsert = false;
        break;
      }
    }
    if(needInsert) indices.push_back(newIndex);
  }

  return indices;
}

SwapChain::SwapChain( LogicalDevice& device,
                      RenderSurface& surface,
                      CommandQueue& presentQueue,
                      std::vector<CommandQueue const*> ownerQueues,
                      PresentMode presentMode) :
  _device(device),
  _presentQueue(presentQueue),
  _handle(VK_NULL_HANDLE),
  _imageFormat(VK_FORMAT_UNDEFINED),
  _extent(0, 0),
  _presentMode(presentMode)
{
  try
  {
    ownerQueues.push_back(&_presentQueue);

    _createSwapchain(surface, ownerQueues);
    _createSyncObjects();
  }
  catch(...)
  {
    _cleanup();
    throw;
  }
}

void SwapChain::_createSwapchain(
                            RenderSurface& surface,
                            const std::vector<CommandQueue const*>& ownerQueues)
{
  if(!_device.physicalDevice().isSurfaceSuitable(surface)) Abort("SwapChain::SwapChain: Device is not suitable with surface.");
  PhysicalDevice::SwapChainSupport swapChainSupport =
                      _device.physicalDevice().surface—ompatibility(surface);

  VkSurfaceFormatKHR surfaceFormat =
                            chooseSwapSurfaceFormat(swapChainSupport.formats);
  _imageFormat = surfaceFormat.format;
  VkPresentModeKHR presentMode = _presentMode == FIFO_PRESENT_MODE ?
                          VK_PRESENT_MODE_FIFO_KHR :
                          chooseSwapPresentMode(swapChainSupport.presentModes);
  VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);
  _extent = glm::ivec2(extent.width, extent.height);
  uint32_t imageCount = chooseImageCount(swapChainSupport.capabilities);

  VkSwapchainCreateInfoKHR createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = surface.handle();
  createInfo.minImageCount = imageCount;
  createInfo.imageFormat = surfaceFormat.format;
  createInfo.imageColorSpace = surfaceFormat.colorSpace;
  createInfo.imageExtent = extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
                          VK_IMAGE_USAGE_TRANSFER_DST_BIT;

  std::vector<uint32_t> familyIndices = getFamilyIndices(ownerQueues);
  if(familyIndices.size() <= 1)
  {
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;
    createInfo.pQueueFamilyIndices = nullptr;
  }
  else
  {
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = uint32_t(familyIndices.size());
    createInfo.pQueueFamilyIndices = familyIndices.data();
  }
  
  createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;
  createInfo.oldSwapchain = VK_NULL_HANDLE;

  if (vkCreateSwapchainKHR( _device.handle(),
                            &createInfo,
                            nullptr,
                            &_handle) != VK_SUCCESS)
  {
    throw std::runtime_error("Failed to create swap chain.");
  }

  if(vkGetSwapchainImagesKHR( _device.handle(),
                              _handle,
                              &imageCount,
                              nullptr) != VK_SUCCESS)
  {
    throw std::runtime_error("Unable to get images count from swapchain.");
  }

  _frames.resize(imageCount);
  std::vector<VkImage> images(imageCount);
  if(vkGetSwapchainImagesKHR( _device.handle(),
                              _handle,
                              &imageCount,
                              images.data()) != VK_SUCCESS)
  {
    throw std::runtime_error("Unable to get images from swapchain.");
  }
  for(size_t frameIndex = 0; frameIndex < _frames.size(); frameIndex++)
  {
    _frames[frameIndex].image = new Image(images[frameIndex],
                                          VK_IMAGE_TYPE_2D,
                                          VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                                          _imageFormat,
                                          glm::uvec3(_extent, 1),
                                          VK_SAMPLE_COUNT_1_BIT,
                                          1,
                                          1,
                                          _device);

    auto initCommand =
      [&](CommandProducer& uploadProducer)
      {
        _frames[frameIndex].image->scheduleInitLayout(VK_IMAGE_ASPECT_COLOR_BIT,
                                                      uploadProducer);
      };
    _device.runTransferCommand(initCommand);
  }
}

void SwapChain::_createSyncObjects()
{
  for(size_t frameIndex = 0; frameIndex < _frames.size(); frameIndex++)
  {
    _frames[frameIndex].endDrawningFence = new Fence(_device);
    _frames[frameIndex].resetFenceSemaphore = new Semaphore(_device);
  }
}

SwapChain::~SwapChain() noexcept
{
  if(_lockedFrameIndex.has_value()) Abort("SwapChain::~SwapChain: swapchain is locked.");
  _cleanup();
}

void SwapChain::_cleanup() noexcept
{
  _presentQueue.waitIdle();

  _frames.clear();

  if(_handle != VK_NULL_HANDLE)
  {
    vkDestroySwapchainKHR(_device.handle(), _handle, nullptr);
    _handle = VK_NULL_HANDLE;
  }
}

size_t SwapChain::_lockFrame(Semaphore& imageAvailableSemaphore)
{
  if(_lockedFrameIndex.has_value()) Abort("SwapChain::_lockFrame: swapchain is already locked.");

  uint32_t frameIndex;
  if(vkAcquireNextImageKHR( _device.handle(),
                            _handle,
                            UINT64_MAX,
                            imageAvailableSemaphore.handle(),
                            VK_NULL_HANDLE,
                            &frameIndex) != VK_SUCCESS)
  {
    throw std::runtime_error("Failed to acquire next image from swapchain.");
  }

  _frames[frameIndex].endDrawningFence->wait();

  _lockedFrameIndex = frameIndex;
  _frames[frameIndex].imageAvailableSemaphore = &imageAvailableSemaphore;

  return _lockedFrameIndex.value();
}

void SwapChain::_unlockFrame(Semaphore& endDrawningSemaphore)
{
  if (!_lockedFrameIndex.has_value()) Abort("SwapChain::_unlockFrame: swapchain is not locked.");
  uint32_t frameIndex = _lockedFrameIndex.value();

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  VkSemaphore waitSemaphores[] = { endDrawningSemaphore.handle()};
  VkPipelineStageFlags waitStages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = &waitStages;
  submitInfo.commandBufferCount = 0;
  submitInfo.pCommandBuffers = nullptr;
  VkSemaphore signalSemaphores[] = {
                            _frames[frameIndex].resetFenceSemaphore->handle()};
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;
  _frames[frameIndex].endDrawningFence->reset();
  if (vkQueueSubmit(
                  _presentQueue.handle(),
                  1,
                  &submitInfo,
                  _frames[frameIndex].endDrawningFence->handle()) != VK_SUCCESS)
  {
    throw std::runtime_error("Failed to submit frame fence.");
  }

  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  VkSemaphore waitSemaphoreHandle =
                              _frames[frameIndex].resetFenceSemaphore->handle();
  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = &waitSemaphoreHandle;
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = &_handle;
  presentInfo.pImageIndices = &frameIndex;
  presentInfo.pResults = nullptr;
  if(vkQueuePresentKHR(_presentQueue.handle(), &presentInfo) != VK_SUCCESS)
  {
    Log() << "SwapChain::_unlockFrame: error in vkQueuePresentKHR";
  }
  
  _frames[frameIndex].endDrawningSemaphore = &endDrawningSemaphore;

  _lockedFrameIndex.reset();
}

void SwapChain::_unlockFrame() noexcept
{
  if (!_lockedFrameIndex.has_value()) Abort("SwapChain::_unlockFrame(noexcept): swapchain is not locked.");
  uint32_t frameIndex = _lockedFrameIndex.value();

  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.waitSemaphoreCount = 0;
  presentInfo.pWaitSemaphores = nullptr;
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = &_handle;
  presentInfo.pImageIndices = &frameIndex;
  presentInfo.pResults = nullptr;
  if(vkQueuePresentKHR(_presentQueue.handle(), &presentInfo) != VK_SUCCESS)
  {
    Log() << "SwapChain::_unlockFrame(noexcept): error in vkQueuePresentKHR";
  }

  _lockedFrameIndex.reset();
}
