#include <memory>
#include <vector>
#include <stdexcept>

#include <glm/vec3.hpp>

#include <mtt/application/ResourceManager/Texture2DLibrary.h>
#include <mtt/application/Widgets/RenderWidget/OrbitalCameraController.h>
#include <mtt/application/Widgets/RenderWidget/RenderWidget.h>
#include <mtt/application/Application.h>
#include <mtt/clPipeline/MeshTechniques/UnlightedColorTechnique.h>
#include <mtt/clPipeline/ColorFrameBuilder.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/render/Mesh/Mesh.h>
#include <mtt/render/Pipeline/Sampler.h>
#include <mtt/render/Pipeline/Texture2D.h>
#include <mtt/render/SceneGraph/CameraNode.h>
#include <mtt/render/RenderScene.h>

class Scene : public mtt::RenderScene
{
private:
  mtt::Mesh mesh;

public:
  Scene(mtt::LogicalDevice& displayDevice) :
    mesh(displayDevice)
  {
    std::vector<glm::vec3> positionsData ={ glm::vec3(-1.f, -1.f, 0.f),
                                            glm::vec3(-1.f,  1.f, 0.f),
                                            glm::vec3( 1.f, -1.f, 0.f),
                                            glm::vec3(-1.f,  1.f, 0.f),
                                            glm::vec3( 1.f,  1.f, 0.f),
                                            glm::vec3( 1.f, -1.f, 0.f) };

    std::shared_ptr<mtt::Buffer> positionBuffer =
      std::make_shared<mtt::Buffer>(displayDevice, mtt::Buffer::VERTEX_BUFFER);
    positionBuffer->setData(positionsData.data(),
                            positionsData.size() * sizeof(positionsData[0]));

    mesh.setPositionBuffer(positionBuffer);

    std::vector<glm::vec2> texCoordData = { glm::vec2(0.f, 1.f),
                                            glm::vec2(0.f, 0.f),
                                            glm::vec2(1.f, 1.f),
                                            glm::vec2(0.f, 0.f),
                                            glm::vec2(1.f, 0.f),
                                            glm::vec2(1.f, 1.f) };
    std::shared_ptr<mtt::Buffer> texCoordBuffer =
      std::make_shared<mtt::Buffer>(displayDevice, mtt::Buffer::VERTEX_BUFFER);
    texCoordBuffer->setData(texCoordData.data(),
                            texCoordData.size() * sizeof(texCoordData[0]));

    mesh.setTexCoordBuffer(texCoordBuffer);

    mesh.setVerticesNumber(positionsData.size());

    std::shared_ptr<mtt::Texture2D> texture =
        mtt::Texture2DLibrary::loadTexture(":/scream.jpg", displayDevice, true);
    std::unique_ptr<mtt::Sampler> sampler =
                  std::make_unique<mtt::Sampler>( mtt::PipelineResource::STATIC,
                                                  displayDevice);
    sampler->setAttachedTexture(texture);
    mesh.extraData().setAlbedoSampler(std::move(sampler));

    mesh.setTechnique(
              mtt::clPipeline::colorFrameType,
              std::make_unique<mtt::clPipeline::UnlightedColorTechnique>(
                                          true,
                                          true,
                                          VK_COMPARE_OP_GREATER_OR_EQUAL,
                                          VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST));
    addUnculledDrawable(mesh);
  }

  ~Scene()
  {
    removeUnculledDrawable(mesh);
  }
};

int main(int argc, char* argv[])
{
  try
  {
    VkPhysicalDeviceFeatures features{};
    mtt::Application application( argc,
                                  argv,
                                  "Mtt mesh example",
                                  { 0, 0, 0 },
                                  VK_API_VERSION_1_2,
                                  features,
                                  false);

    Q_INIT_RESOURCE(shaders);

    mtt::RenderWidget window;
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
    cameraController.setDistance(3.f);
    cameraController.setYAngle(-.5f);

    return application.exec();
  }
  catch (std::exception& error)
  {
    mtt::Log() << error.what();
    return 1;
  }
}
