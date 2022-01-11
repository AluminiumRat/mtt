#include <memory>

#include <mtt/DrawImpl/MeshTechniques/ModelTechniquesFactory.h>

#include <EditorApplication.h>

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
  mmdModelLibrary(std::make_unique<mtt::ModelTechniquesFactory>(
                                          true,
                                          true,
                                          true,
                                          true,
                                          VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST),
                  textureLibrary),
  fbxModelLibrary(std::make_unique<mtt::ModelTechniquesFactory>(
                                          true,
                                          true,
                                          true,
                                          true,
                                          VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST),
                  textureLibrary)
{
  _instance = this;
}
