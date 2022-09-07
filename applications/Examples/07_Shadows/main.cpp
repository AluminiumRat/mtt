#include <stdexcept>

#include <glm/gtx/transform.hpp>

#include <QtCore/QTimer>

#include <mtt/application/DrawModel/MasterDrawModel.h>
#include <mtt/application/Widgets/RenderWidget/OrbitalCameraController.h>
#include <mtt/application/Widgets/RenderWidget/RenderWidget.h>
#include <mtt/application/Application.h>
#include <mtt/clPipeline/Lighting/AmbientLight.h>
#include <mtt/clPipeline/Lighting/PointLight.h>
#include <mtt/clPipeline/MeshTechniques/ModelTechniquesFactory.h>
#include <mtt/clPipeline/ColorFrameBuilder.h>
#include <mtt/render/SceneGraph/CameraNode.h>
#include <mtt/render/RenderScene.h>

class Scene : public mtt::RenderScene
{
private:
  mtt::MasterDrawModel dinoModel;
  mtt::MasterDrawModel fieldModel;
  mtt::clPipeline::AmbientLight ambientLight;
  mtt::clPipeline::PointLight pointLight;
  QTimer updateTimer;
  mtt::TimeT animationTime;

public:
  Scene(mtt::MeshTechniquesFactory& techniquesFactory,
        mtt::LogicalDevice& displayDevice) :
    ambientLight(true, true, true, displayDevice),
    pointLight(true, true, displayDevice),
    dinoModel(":/Dino.mmd", techniquesFactory, nullptr, displayDevice),
    fieldModel(":/crossField.mmd", techniquesFactory, nullptr, displayDevice),
    animationTime(0)
  {
    addCulledDrawable(dinoModel);
    addCulledDrawable(fieldModel);

    ambientLight.setIlluminance(glm::vec3(.5f, .5f, .5f));
    addCompositeObject(ambientLight);

    pointLight.setDistance(10.f);
    pointLight.setShadowmapField(&culledData());
    pointLight.setShadowmapExtent(1024, 256);
    pointLight.setBlurAngle(0.01f);
    addCompositeObject(pointLight);

    updateTimer.setSingleShot(false);
    QTimer::connect(&updateTimer,
                    &QTimer::timeout,
                    [&]
                    {
                      animationTime += mtt::millisecond * 30;
                      float angle = mtt::toFloatTime(animationTime);
                      glm::vec3 position( cos(angle) * 2.f,
                                          sin(angle) * 2.f,
                                          1.f);
                      pointLight.setTransformMatrix(glm::translate(position));
                    });
    updateTimer.start(30);
  }

  ~Scene()
  {
    removeCompositeObject(ambientLight);
    removeCompositeObject(pointLight);
    removeCulledDrawable(dinoModel);
    removeCulledDrawable(fieldModel);
  }
};

int main(int argc, char* argv[])
{
  try
  {
    VkPhysicalDeviceFeatures features{};
    mtt::Application application( argc,
                                  argv,
                                  "Mtt shadows example",
                                  { 0, 0, 0 },
                                  VK_API_VERSION_1_2,
                                  features,
                                  false);

    Q_INIT_RESOURCE(shaders);

    mtt::RenderWidget window;
    window.setFixedSize(800, 600);
    window.show();

    mtt::clPipeline::ModelTechniquesFactory techniquesFactory(
                                          true,
                                          false,
                                          true,
                                          false,
                                          false,
                                          VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
    Scene scene(techniquesFactory, application.displayDevice());

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
    cameraController.setYAngle(-.5f);

    return application.exec();
  }
  catch (std::exception& error)
  {
    mtt::Log() << error.what();
    return 1;
  }
}
