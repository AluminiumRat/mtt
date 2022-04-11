#include <mtt/editorLib/EditorCommonData.h>
#include <mtt/utilities/static_cast_unique_ptr.h>

using namespace mtt;

EditorCommonData::EditorCommonData(UndoStack& undoStack) :
  CommonEditData(undoStack),
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
  setDataFilename(QString());
  emit sceneChanged(newScenePtr);
  return oldScene;

}

void EditorCommonData::setDataFilename(const QString& newValue) noexcept
{
  if (_dataFilename == newValue) return;
  _dataFilename = newValue;
  emit dataFilenameChanged(_dataFilename);
}

void EditorCommonData::setEnvironmentFilename(const QString& newValue) noexcept
{
  if (_environmentFilename == newValue) return;
  _environmentFilename = newValue;
  emit environmentFilenameChanged(_environmentFilename);
}
