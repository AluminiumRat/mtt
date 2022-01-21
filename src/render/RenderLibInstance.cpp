#include <cstdlib>
#include <vector>

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

#include <mtt/render/RenderLibInstance.h>
#include <mtt/utilities/Log.h>

using namespace mtt;

static const char* const validationLayerName = "VK_LAYER_KHRONOS_validation";

static const char* const requiredExtensions[] = { "VK_KHR_surface",
                                                  "VK_KHR_win32_surface" };

RenderLibInstance* RenderLibInstance::_instance = nullptr;

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
                      VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                      VkDebugUtilsMessageTypeFlagsEXT messageType,
                      const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                      void* pUserData)
{
  Log() << "Vulkan validation message: " << pCallbackData->pMessage;
  return VK_FALSE;
}

RenderLibInstance::RenderLibInstance( const char* applicationName,
                                      const Version& applicationVersion,
                                      uint32_t vulkanApiVersion,
                                      bool enableValidation) :
  _handle(VK_NULL_HANDLE),
  _debugMessenger(VK_NULL_HANDLE),
  _isValidationEnabled(enableValidation),
  _vulkanApiVersion(vulkanApiVersion)
{
  if(_instance != nullptr) Abort("RenderLibInstance::RenderLibInstance: RenderLib is already initialized.");

  try
  {
    _createVKInstance(applicationName, applicationVersion);
    _setupDebugMessenger();
    _receivePhysicalDevices();
  }
  catch(...)
  {
    _cleanup();
    throw;
  }

  _instance = this;
}

bool RenderLibInstance::_isValidationSupported() const
{
  uint32_t layerCount = 0;
  if(vkEnumerateInstanceLayerProperties(&layerCount, nullptr) != VK_SUCCESS)
  {
    throw std::runtime_error("Unable to get vk layers count.");
  }

  if(layerCount == 0) return false;

  std::vector<VkLayerProperties> availableLayers(layerCount);
  if(vkEnumerateInstanceLayerProperties(&layerCount,
                                        availableLayers.data()) != VK_SUCCESS)
  {
    throw std::runtime_error("Unable to enumerate vk layers.");
  }

  for(const VkLayerProperties& layer : availableLayers)
  {
    if (strcmp(validationLayerName, layer.layerName) == 0) return true;
  }

  return false;
}

void RenderLibInstance::_createVKInstance(const char* applicationName,
                                          const Version& applicationVersion)
{
  if (_isValidationEnabled && !_isValidationSupported())
  {
    throw std::runtime_error("Validation is not supported.");
  }

  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = applicationName;
  appInfo.applicationVersion = VK_MAKE_VERSION( applicationVersion.major,
                                                applicationVersion.minor,
                                                applicationVersion.patch);
  appInfo.pEngineName = "Renderer engine";
  appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
  appInfo.apiVersion = _vulkanApiVersion;
  
  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  std::vector<const char*> extensions(
          requiredExtensions,
          requiredExtensions +
                  sizeof(requiredExtensions) / sizeof(requiredExtensions[0]));
  if(_isValidationEnabled) extensions.push_back(
                                          VK_EXT_DEBUG_UTILS_EXTENSION_NAME);    
  createInfo.enabledExtensionCount = uint32_t(extensions.size());
  createInfo.ppEnabledExtensionNames = extensions.data();
  
  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
  if (_isValidationEnabled)
  {
    createInfo.enabledLayerCount = 1;
    createInfo.ppEnabledLayerNames = &validationLayerName;

    _populateDebugInfo(debugCreateInfo);
    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
  }
  else createInfo.enabledLayerCount = 0;

  if (vkCreateInstance(&createInfo, nullptr, &_handle) != VK_SUCCESS)
  {
    throw std::runtime_error("Failed to create vk instance.");
  }
}

void RenderLibInstance::_populateDebugInfo(
                                VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
  createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageSeverity =
                            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

  createInfo.messageType =  VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

  createInfo.pfnUserCallback = debugCallback;
}

void RenderLibInstance::_setupDebugMessenger()
{
  if (!_isValidationEnabled) return;

  VkDebugUtilsMessengerCreateInfoEXT createInfo;
  _populateDebugInfo(createInfo);

  PFN_vkCreateDebugUtilsMessengerEXT createFunc =
    (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(
                                            _handle,
                                            "vkCreateDebugUtilsMessengerEXT");
  if(createFunc == nullptr)
  {
    throw std::runtime_error("vkGetInstanceProcAddr is not supported.");
  }

  if (createFunc( _handle,
                  &createInfo,
                  nullptr,
                  &_debugMessenger) != VK_SUCCESS)
  {
    throw std::runtime_error("Unable to create debug messenger.");
  }
}

void RenderLibInstance::_receivePhysicalDevices()
{
  uint32_t deviceCount = 0;
  if(vkEnumeratePhysicalDevices(_handle, &deviceCount, nullptr) != VK_SUCCESS)
  {
    throw std::runtime_error("Unable to get physical device count.");
  }

  if(deviceCount == 0) return;

  _physicalDevices.reserve(deviceCount);

  std::vector<VkPhysicalDevice> devices(deviceCount);
  if(vkEnumeratePhysicalDevices(_handle,
                                &deviceCount,
                                devices.data()) != VK_SUCCESS)
  {
    throw std::runtime_error("Unable to enumerate physical devices.");
  }

  for(VkPhysicalDevice deviceHandle : devices)
  {
    try
    {
      _physicalDevices.push_back(
                        std::make_unique<PhysicalDevice>(*this, deviceHandle));
    }
    catch(const std::runtime_error& error)
    {
      Log() << "Unable to access to physical device " << deviceHandle << ". Reason: " << error.what();
    }
  }
}

RenderLibInstance::~RenderLibInstance()
{
  _cleanup();
}

void RenderLibInstance::_cleanup() noexcept
{
  if (_handle != VK_NULL_HANDLE)
  {
    if(_debugMessenger != VK_NULL_HANDLE)
    {
      PFN_vkDestroyDebugUtilsMessengerEXT destructFunc =
        (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
                                              _handle,
                                              "vkDestroyDebugUtilsMessengerEXT");
      if (destructFunc != nullptr)
      {
        destructFunc(_handle, _debugMessenger, nullptr);
      }
    }

    vkDestroyInstance(_handle, nullptr);
    _handle = VK_NULL_HANDLE;
  }
}
