#include <mtt/utilities/static_cast_unique_ptr.h>

#include <ParticlesEditorCommonData.h>

ParticlesEditorCommonData::ParticlesEditorCommonData() :
  EditorCommonData(_undoStack),
  _undoStack(_animationPlayer)
{
}

std::unique_ptr<ParticlesEditorScene> ParticlesEditorCommonData::setScene(
                                std::unique_ptr<ParticlesEditorScene> newScene)
{
  ParticlesEditorScene* newScenePtr = newScene.get();
  std::unique_ptr<ParticlesEditorScene> oldScene =
                        static_cast_unique_ptr<ParticlesEditorScene>(
                              EditorCommonData::setScene(std::move(newScene)));
  setDataFilename(QString());
  emit sceneChanged(newScenePtr);
  return oldScene;
}
