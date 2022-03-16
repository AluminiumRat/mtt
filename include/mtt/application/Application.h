#pragma once

#include <chrono>
#include <optional>

#include <QtWidgets/QApplication>

#include <mtt/application/WorkCycle/WorkCycle.h>
#include <mtt/application/ApplicationStatistic.h>
#include <mtt/render/LogicalDevice.h>
#include <mtt/render/RenderLibInstance.h>
#include <mtt/utilities/Abort.h>

namespace mtt
{
  class Application : public QApplication
  {
    Q_OBJECT

  public:
    Application(int& argc,
                char** argv,
                const char* applicationName,
                const RenderLibInstance::Version& applicationVersion,
                uint32_t vulkanApiVersion,
                const VkPhysicalDeviceFeatures& displayDeviceFeatures,
                bool enableValidation);
    Application(const Application&) = delete;
    Application& operator = (const Application&) = delete;
    virtual ~Application() noexcept;

    inline static Application& instance() noexcept;

    inline const ApplicationStatistic& statistic() const noexcept;

    inline const LogicalDevice& displayDevice() const noexcept;
    inline LogicalDevice& displayDevice() noexcept;

  public:
    RenderLibInstance renderLibInstance;
    WorkCycle workCycle;

  signals:
    void statisticUpdated(const ApplicationStatistic& statstic);

  protected:
    virtual void timerEvent(QTimerEvent* theEvent) override;

  private:
    void createDisplayDevice(const VkPhysicalDeviceFeatures& features);

  private:
    static Application* _instance;

    size_t _cyclesCounter;
    std::chrono::system_clock::time_point _lastCycleFinish;
    ApplicationStatistic _statistic;

    std::optional<LogicalDevice> _displayDevice;
  };

  inline Application& Application::instance() noexcept
  {
    if(_instance == nullptr) Abort("Application::Instance: application is not crated");
    return *_instance;
  }

  inline const ApplicationStatistic& Application::statistic() const noexcept
  {
    return _statistic;
  }

  inline const LogicalDevice& Application::displayDevice() const noexcept
  {
    return _displayDevice.value();
  }

  inline LogicalDevice& Application::displayDevice() noexcept
  {
    return _displayDevice.value();
  }
}