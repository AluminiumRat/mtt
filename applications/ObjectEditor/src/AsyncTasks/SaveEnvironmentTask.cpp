#include <AsyncTasks/SaveEnvironmentTask.h>
#include <EditorCommonData.h>
#include <ObjectEditorScene.h>

SaveEnvironmentTask::SaveEnvironmentTask( const ObjectEditorScene& scene,
                                          const QString& filename,
                                          EditorCommonData& commonData) :
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
