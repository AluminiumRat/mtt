#define VK_USE_PLATFORM_WIN32_KHR

#include <stdexcept>

#include <QtWidgets/QVBoxLayout>

#include <mtt/application/Widgets/RenderWidget/RenderWidget.h>
#include <mtt/application/Application.h>

using namespace mtt;

RenderWidget::RenderWidget() :
  _device(Application::instance().displayDevice()),
  _windowContainer(nullptr),
  _scene(nullptr),
  _camera(nullptr),
  _renderAction(nullptr),
  _needAction(false)
{
  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->setMargin(0);
  _windowContainer = QWidget::createWindowContainer(&_renderWindow, nullptr);
  layout->addWidget(_windowContainer);

  Application& application = Application::instance();

  VkWin32SurfaceCreateInfoKHR surfaceCreateInfo{};
  surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
  surfaceCreateInfo.hwnd = HWND(_renderWindow.winId());
  surfaceCreateInfo.hinstance = GetModuleHandle(nullptr);
  VkSurfaceKHR surfaceHandle = VK_NULL_HANDLE;
  if(vkCreateWin32SurfaceKHR( application.renderLibInstance.handle(),
                              &surfaceCreateInfo,
                              nullptr,
                              &surfaceHandle) != VK_SUCCESS)
  {
    throw std::runtime_error("RenderWidget: Failed to create render surface");
  }
  _surface.emplace(surfaceHandle);
  if(!application.displayDevice().isSurfaceSuitable(_surface.value()))
  {
    throw std::runtime_error("RenderWidget: Render device is not suitable with surface.");
  }

  _renderer.emplace(_device,
                    _device.drawQueue(),
                    *_device.presentationQueue(),
                    _surface.value());

  resize(800, 600);
}

RenderWidget::~RenderWidget() noexcept
{
  removeAction();
}

void RenderWidget::setPresentMode(SwapChain::PresentMode newValue) noexcept
{
  if (presentMode() == newValue) return;
  _renderer->setPresentMode(newValue);
  emit presentModeChanged(newValue);
}

void RenderWidget::removeAction() noexcept
{
  if (_renderAction == nullptr) return;
  Application& application = Application::instance();
  application.workCycle.removeAction(*_renderAction);
  _renderAction = nullptr;
}

void RenderWidget::setSource(RenderScene* scene, CameraNode* camera)
{
  rebuildAction(scene, camera);
  _scene = scene;
  _camera = camera;

  emit cameraChanged(_camera);
  emit sceneChanged(_scene);
}

void RenderWidget::setCamera(CameraNode* camera)
{
  rebuildAction(_scene, camera);
  _camera = camera;
  emit cameraChanged(_camera);
}

std::unique_ptr<RenderSceneAction> RenderWidget::buildRenderAction(
                                              RenderScene& scene,
                                              CameraNode& camera,
                                              SceneToSurfaceRenderer& renderer)
{
  return std::make_unique<RenderSceneAction>(renderer, scene, camera, nullptr);
}

void RenderWidget::rebuildAction(RenderScene* scene, CameraNode* camera)
{
  removeAction();

  if(!_needAction || scene == nullptr || camera == nullptr) return;

  std::unique_ptr<RenderSceneAction> action = buildRenderAction(
                                                            *scene,
                                                            *camera,
                                                            _renderer.value());
  RenderSceneAction* actionPtr = action.get();
  Application& application = Application::instance();
  application.workCycle.addAction(std::move(action),
                                  WorkCycle::RENDER_CATEGORY,
                                  WorkCycle::RENDER_PRIORITY,
                                  true);
  _renderAction = actionPtr;
}

void RenderWidget::showEvent(QShowEvent* event)
{
  QWidget::showEvent(event);
  _needAction = true;
  try
  {
    rebuildAction(_scene, _camera);
  }
  catch(std::exception error)
  {
    Log() << "RenderWidget::showEvent: " << error.what();
  }
  catch(...)
  {
    Log() << "RenderWidget::showEvent: unable to create render action";
  }
}

void RenderWidget::hideEvent(QHideEvent* event)
{
  QWidget::hideEvent(event);

  _needAction = false;
  try
  {
    removeAction();
  }
  catch (std::exception error)
  {
    Log() << "RenderWidget::hideEvent: " << error.what();
  }
  catch (...)
  {
    Log() << "RenderWidget::hideEvent: unable to remove render action";
  }
}
