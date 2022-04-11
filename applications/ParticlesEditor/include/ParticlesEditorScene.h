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
