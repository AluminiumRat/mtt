#include <mtt/Utilities/static_cast_unique_ptr.h>

#include <EditorCommonData.h>

EditorCommonData::EditorCommonData() :
  CommonEditData(_undoStack),
  _undoStack(_animationPlayer),
  _renderScene(nullptr)
{
}

std::unique_ptr<EditorScene> EditorCommonData::setScene(
                                          std::unique_ptr<EditorScene> newScene)
{
  EditorScene* newScenePtr = newScene.get();
  std::unique_ptr<EditorScene> oldScene =
                          static_cast_unique_ptr<EditorScene>(
                                CommonEditData::setScene(std::move(newScene)));
  emit sceneChanged(newScenePtr);
  return oldScene;
}
