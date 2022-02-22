#include <mtt/utilities/static_cast_unique_ptr.h>

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
  setEffectFilename(QString());
  emit sceneChanged(newScenePtr);
  return oldScene;
}

void EditorCommonData::setEffectFilename(const QString& newValue) noexcept
{
  if(_effectFilename == newValue) return;
  _effectFilename = newValue;
  emit effectFilenameChanged(_effectFilename);
}
