#pragma once

#include <mtt/editorLib/EditorScene.h>

#include <Objects/RootObject.h>

class ObjectEditorScene : public mtt::EditorScene
{
  Q_OBJECT

public:
  ObjectEditorScene();
  ObjectEditorScene(const ObjectEditorScene&) = delete;
  ObjectEditorScene& operator = (const ObjectEditorScene&) = delete;
  virtual ~ObjectEditorScene() noexcept = default;

  inline RootObject& dataRoot() noexcept;
  inline const RootObject& dataRoot() const noexcept;

signals:
  void dataRootChanged(RootObject& newDataRoot);

private:
  RootObject* _dataRoot;
};

inline RootObject& ObjectEditorScene::dataRoot() noexcept
{
  return *_dataRoot;
}

inline const RootObject& ObjectEditorScene::dataRoot() const noexcept
{
  return *_dataRoot;
}
