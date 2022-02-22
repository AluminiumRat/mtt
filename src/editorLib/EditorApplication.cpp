#include <memory>

#include <mtt/clPipeline/MeshTechniques/ModelTechniquesFactory.h>
#include <mtt/editorLib/EditorApplication.h>

using namespace mtt;

#ifdef NDEBUG
  static const bool enableValidation = false;
#else
  static const bool enableValidation = true;
#endif

static VkPhysicalDeviceFeatures deviceFeatures()
{
  VkPhysicalDeviceFeatures features{};
  features.samplerAnisotropy = VK_TRUE;
  return features;
}

EditorApplication* EditorApplication::_instance = nullptr;

EditorApplication::EditorApplication(int& argc, char** argv) :
  Application(argc,
              argv,
              "Mtt object editor",
              { 0, 0, 0 },
              VK_API_VERSION_1_2,
              deviceFeatures(),
              enableValidation),
  mmdModelLibrary(std::make_unique<mtt::clPipeline::ModelTechniquesFactory>(
                                          true,
                                          true,
                                          true,
                                          true,
                                          true,
                                          VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST),
                  textureLibrary),
  fbxModelLibrary(std::make_unique<mtt::clPipeline::ModelTechniquesFactory>(
                                          true,
                                          true,
                                          true,
                                          true,
                                          true,
                                          VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST),
                  textureLibrary)
{
  Q_INIT_RESOURCE(editorLibResources);
  _instance = this;
}
