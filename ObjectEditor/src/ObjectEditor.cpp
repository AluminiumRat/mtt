#include <EditorApplication.h>

#include <MainWindow.h>

int main(int argc, char* argv[])
{
  EditorApplication application(argc, argv);
  
  MainWindow mainWindow;
  mainWindow.show();
  
  return application.exec();
}
