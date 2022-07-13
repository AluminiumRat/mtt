#pragma once

#include <mtt/editorLib/EditorScene.h>

#include <Objects/RootObject.h>

class ParticlesEditorScene : public mtt::EditorScene
{
  Q_OBJECT

public:
  ParticlesEditorScene();
  ParticlesEditorScene(const ParticlesEditorScene&) = delete;
  ParticlesEditorScene& operator = (const ParticlesEditorScene&) = delete;
  virtual ~ParticlesEditorScene() noexcept = default;

  inline RootObject& dataRoot() noexcept;
  inline const RootObject& dataRoot() const noexcept;
  /// newRoot must not be nullptr
  /// returns old data root
  std::unique_ptr<RootObject> changeDataRoot(
                                  std::unique_ptr<RootObject> newRoot) noexcept;

signals:
  void dataRootChanged(RootObject& newDataRoot);

private:
  RootObject* _dataRoot;
};

inline RootObject& ParticlesEditorScene::dataRoot() noexcept
{
  return *_dataRoot;
}

inline const RootObject& ParticlesEditorScene::dataRoot() const noexcept
{
  return *_dataRoot;
}
