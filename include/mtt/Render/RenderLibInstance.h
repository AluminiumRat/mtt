#pragma once

#include <memory>
#include <vector>

#include <vulkan/vulkan.h>

#include <mtt/Render/PhysicalDevice.h>
#include <mtt/Utilities/Abort.h>
#include <mtt/Utilities/StringRegistry.h>

namespace mtt
{
  class RenderLibInstance
  {
  public:
    struct Version
    {
      uint32_t major = 1;
      uint32_t minor = 0;
      uint32_t patch = 0;
    };

  public:
    RenderLibInstance(const char* applicationName,
                      const Version& applicationVersion,
                      uint32_t vulkanApiVersion,
                      bool enableValidation);
    RenderLibInstance(const RenderLibInstance&) = delete;
    RenderLibInstance& operator = (const RenderLibInstance&) = delete;
    virtual ~RenderLibInstance();

    inline static RenderLibInstance& instance() noexcept;

    inline VkInstance handle() const;

    inline uint32_t vulkanApiVersion() const noexcept;

    inline bool isValidationEnabled() const;

    inline size_t physicalDeviceCount() const;
    inline const PhysicalDevice& physicalDevice(size_t index) const;
    inline PhysicalDevice& physicalDevice(size_t index);

  public:
    StringRegistry namesRegistry;

  private:
    void _cleanup() noexcept;
    bool _isValidationSupported() const;
    void _createVKInstance( const char* applicationName,
                            const Version& applicationVersion);
    static void _populateDebugInfo(
                                VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    void _setupDebugMessenger();
    void _receivePhysicalDevices();

  private:
    VkInstance _handle;
    VkDebugUtilsMessengerEXT _debugMessenger;
    bool _isValidationEnabled;

    uint32_t _vulkanApiVersion;

    using PhysicalDevices = std::vector<std::unique_ptr<PhysicalDevice>>;
    PhysicalDevices _physicalDevices;

    static RenderLibInstance* _instance;
  };

  inline RenderLibInstance& RenderLibInstance::instance() noexcept
  {
    if(_instance == nullptr) Abort("RenderLibInstance::instance: RenderLib is not initialized.");
    return *_instance;
  }

  inline VkInstance RenderLibInstance::handle() const
  {
    return _handle;
  }

  inline uint32_t RenderLibInstance::vulkanApiVersion() const noexcept
  {
    return _vulkanApiVersion;
  }

  inline bool RenderLibInstance::isValidationEnabled() const
  {
    return _isValidationEnabled;
  }

  inline size_t RenderLibInstance::physicalDeviceCount() const
  {
    return _physicalDevices.size();
  }

  inline const PhysicalDevice& RenderLibInstance::physicalDevice(
                                                            size_t index) const
  {
    return *_physicalDevices[index];
  }

  inline PhysicalDevice& RenderLibInstance::physicalDevice(size_t index)
  {
    return *_physicalDevices[index];
  }
}