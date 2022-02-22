#include <AsyncTasks/SaveEnvironmentTask.h>
#include <EditorCommonData.h>
#include <EditorScene.h>

SaveEnvironmentTask::SaveEnvironmentTask( const EditorScene& scene,
                                          const QString& filename,
                                          EditorCommonData& commonData) :
  mtt::SaveEnvironmentTask( filename,
                            scene.root().environment(),
                            scene.root().background()),
  _commonData(commonData)
{
}

void SaveEnvironmentTask::finalizePart()
{
  mtt::SaveEnvironmentTask::finalizePart();
  _commonData.setEnvironmentFilename(filename());
}
