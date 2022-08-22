#include <stdexcept>

#include <glm/gtx/transform.hpp>

#include <QtCore/QTimer>

#include <mtt/application/Widgets/RenderWidget/OrbitalCameraController.h>
#include <mtt/application/Widgets/RenderWidget/RenderWidget.h>
#include <mtt/application/Application.h>
#include <mtt/clPipeline/Lighting/PointLight.h>
#include <mtt/clPipeline/ColorFrameBuilder.h>
#include <mtt/particles/PSTEffect/PSTEffect.h>
#include <mtt/render/SceneGraph/CameraNode.h>
#include <mtt/render/RenderScene.h>

class Scene : public mtt::RenderScene
{
private:
  mtt::clPipeline::PointLight light;
  mtt::PSTEffect effect;
  QTimer updateTimer;

public:
  Scene(mtt::LogicalDevice& displayDevice) :
    light(true, true, displayDevice),
    effect(":/fire.pst", nullptr, displayDevice)
  {
    light.setDistance(5);
    light.setIlluminance(glm::vec3(.225f, .12f, .04f));

    updateTimer.setSingleShot(false);
    QTimer::connect(&updateTimer,
                    &QTimer::timeout,
                    [&]
                    {
                      effect.setTime(
                                  effect.currentTime() + 30 * mtt::millisecond);
                    });
    updateTimer.start(30);

    addCompositeObject(light);
    addCulledDrawable(effect);
  }

  ~Scene()
  {
    removeCompositeObject(light);
    removeCulledDrawable(effect);
  }
};

int main(int argc, char* argv[])
{
  try
  {
    VkPhysicalDeviceFeatures features{};
    features.geometryShader = VK_TRUE;

    mtt::Application application( argc,
                                  argv,
                                  "Mtt particles example",
                                  { 0, 0, 0 },
                                  VK_API_VERSION_1_2,
                                  features,
                                  false);

    mtt::RenderWidget window;
    window.setFixedSize(800, 600);
    window.show();

    Scene scene(application.displayDevice());

    mtt::CameraNode camera;
    camera.setPerspectiveProjection(glm::pi<float>() / 2.f, 1.33f, 0.1f, 50.f);

    window.setSource(&scene, &camera);

    mtt::clPipeline::ColorFrameBuilder colorFrameBuilder(
                                                  window.surfaceFormat(),
                                                  application.displayDevice());
    window.setFrameBuilder(&colorFrameBuilder);

    mtt::OrbitalCameraController cameraController(window, nullptr);
    cameraController.setCenterPosition(glm::vec3(0.f, 0.f, 0.5f));
    cameraController.setDistance(1.5f);
    cameraController.setYAngle(0.f);

    return application.exec();
  }
  catch (std::exception& error)
  {
    mtt::Log() << error.what();
    return 1;
  }
}
