#pragma once

#include <mtt/application/Scene/Scene.h>

#include <Objects/RootObject.h>

class EditorScene : public mtt::Scene
{
  Q_OBJECT

public:
  EditorScene();
  EditorScene(const EditorScene&) = delete;
  EditorScene& operator = (const EditorScene&) = delete;
  virtual ~EditorScene() noexcept;

  inline RootObject& root() noexcept;
  inline const RootObject& root() const noexcept;

private:
  RootObject _root;
};

inline RootObject& EditorScene::root() noexcept
{
  return _root;
}

inline const RootObject& EditorScene::root() const noexcept
{
  return _root;
}
