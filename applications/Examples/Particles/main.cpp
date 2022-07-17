#include <glm/gtx/transform.hpp>

#include <QtCore/QTimer>

#include <mtt/application/Widgets/RenderWidget/OrbitalCameraController.h>
#include <mtt/application/Widgets/RenderWidget/RenderWidget.h>
#include <mtt/application/Application.h>
#include <mtt/clPipeline/Lighting/DirectLight.h>
#include <mtt/clPipeline/ColorFrameBuilder.h>
#include <mtt/particles/PSTEffect/PSTEffect.h>
#include <mtt/render/SceneGraph/CameraNode.h>
#include <mtt/render/RenderScene.h>

int main(int argc, char* argv[])
{
  VkPhysicalDeviceFeatures features{};
  features.samplerAnisotropy = VK_TRUE;
  features.geometryShader = VK_TRUE;

  mtt::Application application( argc,
                                argv,
                                "Mtt particles example",
                                { 0, 0, 0 },
                                VK_API_VERSION_1_2,
                                features,
                                true);

  mtt::RenderWidget window;
  window.setFixedSize(800, 600);
  window.show();

  mtt::CameraNode camera;
  camera.setPerspectiveProjection(glm::pi<float>() / 2.f, 1.33f, 0.1f, 50.f);

  mtt::clPipeline::DirectLight light(true, true, application.displayDevice());
  light.setTransformMatrix(glm::translate(glm::vec3(0.f, 0.f, 10.f)));

  mtt::PSTEffect effect("C:/Projects/models/pst/1.pst",
                        nullptr,
                        application.displayDevice());
  QTimer updateTimer;
  updateTimer.setSingleShot(false);
  QTimer::connect(&updateTimer,
                  &QTimer::timeout,
                  [&]
                  {
                    effect.setTime(
                                effect.currentTime() + 30 * mtt::millisecond);
                  });
  updateTimer.start(30);

  mtt::RenderScene scene;
  scene.addCompositeObject(light);
  scene.addCulledDrawable(effect);

  light.setShadowmapField(&scene.culledData());
  light.setShadowmapExtent(512, 512);

  window.setSource(&scene, &camera);

  mtt::clPipeline::ColorFrameBuilder colorFrameBuilder(
                                                  window.surfaceFormat(),
                                                  application.displayDevice());
  window.setFrameBuilder(&colorFrameBuilder);

  mtt::OrbitalCameraController cameraController(window, nullptr);
  cameraController.setCenterPosition(glm::vec3(0.f, 0.f, 0.5f));
  cameraController.setDistance(1.f);
  cameraController.setYAngle(0.f);

  return application.exec();
}
