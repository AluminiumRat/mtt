#include <mtt/editorLib/EditorApplication.h>

#include <MainWindow.h>

int main(int argc, char* argv[])
{
  mtt::EditorApplication application( argc,
                                      argv,
                                      "Mtt object editor",
                                      { 0, 0, 0 });
  MainWindow mainWindow;
  mainWindow.show();

  return application.exec();
}
