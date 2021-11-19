#define VK_USE_PLATFORM_WIN32_KHR

#include <stdexcept>

#include <QtGui/QWindow>

#include <mtt/Core/Application.h>
#include <mtt/Render/RenderSurface.h>

using namespace mtt;

Application* Application::_instance = nullptr;
Application::Application( int& argc,
                          char** argv,
                          const char* applicationName,
                          const RenderLibInstance::Version& applicationVersion,
                          uint32_t vulkanApiVersion,
                          const VkPhysicalDeviceFeatures& displayDeviceFeatures,
                          bool enableValidation) :
  QApplication(argc, argv),
  renderLibInstance(applicationName,
                    applicationVersion,
                    vulkanApiVersion,
                    enableValidation),
  _cyclesCounter(0)
{
  if (_instance != nullptr) Abort("Application::Application: application is already created");
  _instance = this;

  createDisplayDevice(displayDeviceFeatures);

  startTimer(0);
}

void Application::createDisplayDevice(const VkPhysicalDeviceFeatures& features)
{
  QWindow testWindow;

  VkWin32SurfaceCreateInfoKHR surfaceCreateInfo{};
  surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
  surfaceCreateInfo.hwnd = HWND(testWindow.winId());
  surfaceCreateInfo.hinstance = GetModuleHandle(nullptr);
  VkSurfaceKHR surfaceHandle = VK_NULL_HANDLE;
  if(vkCreateWin32SurfaceKHR( renderLibInstance.handle(),
                              &surfaceCreateInfo,
                              nullptr,
                              &surfaceHandle) != VK_SUCCESS)
  {
    throw std::runtime_error("Failed to create Vulkan surface");
  }
  RenderSurface surface(renderLibInstance, surfaceHandle);

  PhysicalDevice* gpu = nullptr;
  for(size_t gpuIndex = 0;
      gpuIndex < renderLibInstance.physicalDeviceCount();
      gpuIndex++)
  {
    PhysicalDevice& currentGpu = renderLibInstance.physicalDevice(gpuIndex);
    if(currentGpu.isSurfaceSuitable(surface))
    {
      gpu = & currentGpu;
      break;
    }
  }
  if(gpu == nullptr) throw std::runtime_error("No suitable GPU found");

  _displayDevice.emplace( *gpu,
                          features,
                          &surface);
}

Application::~Application() noexcept
{
}

void Application::timerEvent(QTimerEvent* theEvent)
{
  workCycle.step();

  _cyclesCounter++;

  std::chrono::system_clock::time_point finishTime =
                                              std::chrono::system_clock::now();
  std::chrono::seconds currentSeconds =
                          std::chrono::duration_cast<std::chrono::seconds>(
                                                finishTime.time_since_epoch());
  std::chrono::seconds lastSeconds =
                          std::chrono::duration_cast<std::chrono::seconds>(
                                          _lastCycleFinish.time_since_epoch());
  if(currentSeconds != lastSeconds)
  {
    _statistic.cyclesPerSecond = _cyclesCounter /
                                        (currentSeconds - lastSeconds).count();
    _cyclesCounter = 0;
  }

  emit statisticUpdated(_statistic);

  _lastCycleFinish = finishTime;
}
