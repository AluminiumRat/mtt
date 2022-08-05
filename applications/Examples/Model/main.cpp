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

    mtt::RenderWidget window;
    window.setFixedSize(800, 600);
    window.show();

    mtt::clPipeline::AmbientLight ambientLight( true,
                                                true,
                                                true,
                                                application.displayDevice());
    ambientLight.setIlluminance(glm::vec3(.5f, .5f, .5f));

    mtt::clPipeline::DirectLight directLight( true,
                                              true,
                                              application.displayDevice());
    directLight.setTransformMatrix(glm::translate(glm::vec3(0.f, 0.f, 3.f)));
    directLight.setHeight(5.f);
    directLight.setDirection(glm::vec3(1.f, 1.f, -1.f));
    directLight.setIlluminance(glm::vec3(2.5f, 2.5f, 2.5f));

    mtt::clPipeline::ModelTechniquesFactory techniquesFactory(
                                          true,
                                          false,
                                          false,
                                          false,
                                          false,
                                          VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
    mtt::MasterDrawModel drawModel( ":/Dino.mmd",
                                    techniquesFactory,
                                    nullptr,
                                    application.displayDevice());
    mtt::DrawModelAnimation* animation = drawModel.findAnimation("Walk");
    if (animation == nullptr) mtt::Log() << "Animation not found";

    mtt::TimeT animationTime(0);
    QTimer updateTimer;
    updateTimer.setSingleShot(false);
    QTimer::connect(&updateTimer,
                    &QTimer::timeout,
                    [&]
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

    mtt::RenderScene scene;
    scene.addCompositeObject(ambientLight);
    scene.addCompositeObject(directLight);
    scene.addCulledDrawable(drawModel);

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
