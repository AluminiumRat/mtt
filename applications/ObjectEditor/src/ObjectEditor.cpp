#include <mtt/editorLib/EditorApplication.h>

#include <MainWindow.h>

int main(int argc, char* argv[])
{
  try
  {
    VkPhysicalDeviceFeatures features{};
    features.samplerAnisotropy = VK_TRUE;
    features.wideLines = VK_TRUE;

    mtt::EditorApplication application( argc,
                                        argv,
                                        "Mtt object editor",
                                        { 0, 0, 0 },
                                        features);

    Q_INIT_RESOURCE(shaders);

    MainWindow mainWindow;
    mainWindow.show();

    return application.exec();
  }
  catch (std::exception& error)
  {
    mtt::Log() << error.what();
    return 1;
  }
}
