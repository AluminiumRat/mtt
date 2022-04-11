#include <AsyncTasks/SaveEnvironmentTask.h>
#include <ObjectEditorCommonData.h>
#include <ObjectEditorScene.h>

SaveEnvironmentTask::SaveEnvironmentTask( const ObjectEditorScene& scene,
                                          const QString& filename,
                                          ObjectEditorCommonData& commonData) :
  mtt::SaveEnvironmentTask( filename,
                            scene.environmentRoot().objects(),
                            scene.environmentRoot().background()),
  _commonData(commonData)
{
}

void SaveEnvironmentTask::finalizePart()
{
  mtt::SaveEnvironmentTask::finalizePart();
  _commonData.setEnvironmentFilename(filename());
}
