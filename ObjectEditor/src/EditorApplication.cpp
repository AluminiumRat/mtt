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
              "Renderer object editor",
              { 0, 0, 0 },
              VK_API_VERSION_1_2,
              deviceFeatures(),
              enableValidation)
{
  _instance = this;
}
