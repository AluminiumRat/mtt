#include <stdexcept>

#include <glm/gtx/transform.hpp>

#include <QtCore/QTimer>

#include <mtt/application/DrawModel/MasterDrawModel.h>
#include <mtt/application/Widgets/RenderWidget/OrbitalCameraController.h>
#include <mtt/application/Widgets/RenderWidget/RenderWidget.h>
#include <mtt/application/Application.h>
#include <mtt/clPipeline/Lighting/AmbientLight.h>
#include <mtt/clPipeline/Lighting/DirectLight.h>
#include <mtt/clPipeline/MeshTechniques/ModelTechniquesFactory.h>
#include <mtt/clPipeline/ColorFrameBuilder.h>
#include <mtt/render/SceneGraph/CameraNode.h>
#include <mtt/render/RenderScene.h>

class Scene : public mtt::RenderScene
{
private:
  mtt::MasterDrawModel drawModel;
  QTimer updateTimer;
  mtt::TimeT animationTime;
  mtt::clPipeline::AmbientLight ambientLight;
  mtt::clPipeline::DirectLight directLight;

public:
  Scene(mtt::MeshTechniquesFactory& techniquesFactory,
        mtt::LogicalDevice& displayDevice) :
    drawModel(":/Dino.mmd", techniquesFactory, nullptr, displayDevice),
    animationTime(0),
    ambientLight(true, true, true, displayDevice),
    directLight(true, true, displayDevice)
  {
    mtt::DrawModelAnimation* animation = drawModel.findAnimation("Walk");
    if (animation == nullptr) mtt::Log() << "Animation not found";
    else
    {
      updateTimer.setSingleShot(false);
      QTimer::connect(&updateTimer,
                      &QTimer::timeout,
                      [&, animation]
                      {
                        if(animation != nullptr)
                        {
                          drawModel.updateFromAnimation(
                                *animation,
                                animation->timeRange().start() + animationTime);
                          animationTime += 30 * mtt::millisecond;
                          animationTime %= animation->timeRange().length();
                        }
                      });
      updateTimer.start(30);
    }
    addCulledDrawable(drawModel);

    ambientLight.setIlluminance(glm::vec3(.5f, .5f, .5f));
    addCompositeObject(ambientLight);

    directLight.setTransformMatrix(glm::translate(glm::vec3(0.f, 0.f, 3.f)));
    directLight.setHeight(5.f);
    directLight.setDirection(glm::vec3(1.f, 1.f, -1.f));
    directLight.setIlluminance(glm::vec3(2.5f, 2.5f, 2.5f));
    addCompositeObject(directLight);
  }

  ~Scene()
  {
    removeCulledDrawable(drawModel);
    removeCompositeObject(ambientLight);
    removeCompositeObject(directLight);
  }
};

int main(int argc, char* argv[])
{
  try
  {
    VkPhysicalDeviceFeatures features{};
    mtt::Application application( argc,
                                  argv,
                                  "Mtt model example",
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
                                          false,
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
