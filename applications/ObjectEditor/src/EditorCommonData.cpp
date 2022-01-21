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
  setModelFilename(QString());
  emit sceneChanged(newScenePtr);
  return oldScene;
}

void EditorCommonData::setModelFilename(const QString& newValue) noexcept
{
  if(_modelFilename == newValue) return;
  _modelFilename = newValue;
  emit modelFilenameChanged(_modelFilename);
}

void EditorCommonData::setEnvironmentFilename(const QString& newValue) noexcept
{
  if(_environmentFilename == newValue) return;
  _environmentFilename = newValue;
  emit environmentFilenameChanged(_environmentFilename);
}
