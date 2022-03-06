#include <mtt/editorLib/EditorApplication.h>

#include <MainWindow.h>

int main(int argc, char* argv[])
{
  VkPhysicalDeviceFeatures features{};
  features.samplerAnisotropy = VK_TRUE;
  features.geometryShader = VK_TRUE;

  mtt::EditorApplication application( argc,
                                      argv,
                                      "Mtt particles editor",
                                      { 0, 0, 0 },
                                      features);

  MainWindow mainWindow;
  mainWindow.show();

  return application.exec();
}
