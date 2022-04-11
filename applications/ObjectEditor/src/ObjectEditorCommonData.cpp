#include <mtt/utilities/static_cast_unique_ptr.h>

#include <ObjectEditorCommonData.h>

ObjectEditorCommonData::ObjectEditorCommonData() :
  EditorCommonData(_undoStack),
  _undoStack(_animationPlayer)
{
}

std::unique_ptr<ObjectEditorScene> ObjectEditorCommonData::setScene(
                                    std::unique_ptr<ObjectEditorScene> newScene)
{
  ObjectEditorScene* newScenePtr = newScene.get();
  std::unique_ptr<ObjectEditorScene> oldScene =
                          static_cast_unique_ptr<ObjectEditorScene>(
                              EditorCommonData::setScene(std::move(newScene)));
  setDataFilename(QString());
  emit sceneChanged(newScenePtr);
  return oldScene;
}
