#include <memory>
#include <stdexcept>
#include <vector>

#include <glm/gtx/transform.hpp>

#include <mtt/application/DrawModel/MasterDrawModel.h>
#include <mtt/application/DrawModel/SlaveDrawModel.h>
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
  mtt::clPipeline::AmbientLight ambientLight;
  mtt::clPipeline::DirectLight directLight;
  std::shared_ptr<mtt::MasterDrawModel> masterModel;
  std::vector<std::unique_ptr<mtt::SlaveDrawModel>> slaveModels;

public:
  Scene(mtt::MeshTechniquesFactory& techniquesFactory,
        mtt::LogicalDevice& displayDevice) :
    ambientLight(true, true, true, displayDevice),
    directLight(true, true, displayDevice),
    masterModel(new mtt::MasterDrawModel( ":/Dino.mmd",
                                          techniquesFactory,
                                          nullptr,
                                          displayDevice))
  {
    for (int modelIndex = 0; modelIndex < 4; modelIndex++)
    {
      std::unique_ptr <mtt::SlaveDrawModel> newModel(
                                          new mtt::SlaveDrawModel(masterModel));
      newModel->setTransformMatrix(glm::translate(glm::vec3(0.f,
                                                            2.f - modelIndex,
                                                            0.f)));
      if (newModel->animationsNumber() != 0)
      {
        const mtt::DrawModelAnimation& animation = newModel->animation(
                                    modelIndex % newModel->animationsNumber());
        newModel->updateFromAnimation(animation,
                                      animation.timeRange().length() / 2);
      }
      slaveModels.push_back(std::move(newModel));
      addCulledDrawable(*slaveModels.back());
    }

    ambientLight.setIlluminance(glm::vec3(.5f, .5f, .5f));

    directLight.setTransformMatrix(glm::translate(glm::vec3(0.f, 0.f, 3.f)));
    directLight.setHeight(5.f);
    directLight.setDirection(glm::vec3(1.f, 1.f, -1.f));
    directLight.setIlluminance(glm::vec3(2.5f, 2.5f, 2.5f));

    addCompositeObject(ambientLight);
    addCompositeObject(directLight);
  }

  ~Scene()
  {
    for (std::unique_ptr<mtt::SlaveDrawModel>& model : slaveModels)
    {
      removeCulledDrawable(*model);
    }

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
