#include <stdexcept>

#include <glm/gtx/transform.hpp>

#include <mtt/application/Manipulator/ManipulatorController.h>
#include <mtt/application/Manipulator/PlaneMove3DManipulator.h>
#include <mtt/application/DrawModel/MasterDrawModel.h>
#include <mtt/application/Widgets/RenderWidget/OrbitalCameraController.h>
#include <mtt/application/Widgets/RenderWidget/RenderWidget.h>
#include <mtt/application/Application.h>
#include <mtt/clPipeline/Lighting/AmbientLight.h>
#include <mtt/clPipeline/MeshTechniques/ModelTechniquesFactory.h>
#include <mtt/clPipeline/MeshTechniques/UnlightedColorTechnique.h>
#include <mtt/clPipeline/ColorFrameBuilder.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/render/SceneGraph/CameraNode.h>
#include <mtt/render/RenderScene.h>

class Scene : public mtt::RenderScene
{
public:
  mtt::clPipeline::AmbientLight ambientLight;
  mtt::MasterDrawModel dinoModel;
  mtt::MasterDrawModel fieldModel;

  Scene(mtt::MeshTechniquesFactory& techniquesFactory,
        mtt::LogicalDevice& displayDevice) :
    ambientLight(true, true, true, displayDevice),
    dinoModel(":/Dino.mmd", techniquesFactory, nullptr, displayDevice),
    fieldModel(":/crossField.mmd", techniquesFactory, nullptr, displayDevice)
  {
    ambientLight.setIlluminance(glm::vec3(1.f, 1.f, 1.f));

    addCompositeObject(ambientLight);
    addCulledDrawable(dinoModel);
    addCulledDrawable(fieldModel);
  }

  ~Scene()
  {
    removeCompositeObject(ambientLight);
    removeCulledDrawable(dinoModel);
    removeCulledDrawable(fieldModel);
  }
};

class Manipulator : public mtt::PlaneMove3DManipulator
{
private:
  mtt::AbstractNode& _movableObject;

public:
  Manipulator(mtt::AbstractNode& movableObject) :
    PlaneMove3DManipulator(mtt::AutoscaleDrawableModificator::PIXEL_SCALE_MODE),
    _movableObject(movableObject)
  {
    std::vector<glm::vec3> positions;
    positions.reserve(12);

    const glm::vec3 p1(-20.f, -20.f, 0.f);
    const glm::vec3 p2(-20.f,  20.f, 0.f);
    const glm::vec3 p3( 20.f,  20.f, 0.f);
    const glm::vec3 p4( 20.f, -20.f, 0.f);

    positions.push_back(p1);
    positions.push_back(p2);
    positions.push_back(p3);

    positions.push_back(p1);
    positions.push_back(p3);
    positions.push_back(p4);

    positions.push_back(p1);
    positions.push_back(p3);
    positions.push_back(p2);

    positions.push_back(p1);
    positions.push_back(p4);
    positions.push_back(p3);

    setGeometry(positions);

    setTechnique( mtt::clPipeline::colorFrameType,
                  std::make_unique<mtt::clPipeline::UnlightedColorTechnique>(
                                          false,
                                          false,
                                          VK_COMPARE_OP_ALWAYS,
                                          VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST));

    node().setTransformMatrix(_movableObject.transformMatrix());
  }

  virtual void processShift(const glm::vec3& startTouchPoint,
                            const glm::vec3& shiftedPoint) noexcept override
  {
    _movableObject.setTransformMatrix(glm::translate(shiftedPoint));
    node().setTransformMatrix(_movableObject.transformMatrix());
  }
};

int main(int argc, char* argv[])
{
  try
  {
    VkPhysicalDeviceFeatures features{};
    mtt::Application application( argc,
                                  argv,
                                  "Mtt manipulator example",
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

    Manipulator manipulator(scene.dinoModel);

    mtt::ManipulatorController manipulatorController(window);
    manipulatorController.setManipulator(&manipulator);

    return application.exec();
  }
  catch (std::exception& error)
  {
    mtt::Log() << error.what();
    return 1;
  }
}
