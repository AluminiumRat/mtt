#pragma once

#include <mtt/Core/CommonEditData.h>
#include <mtt/Render/RenderScene.h>

#include <AnimationPlayer.h>
#include <EditorScene.h>
#include <EditorUndoStack.h>

class EditorCommonData : public mtt::CommonEditData
{
  Q_OBJECT

public:
  EditorCommonData();
  EditorCommonData(const EditorCommonData&) = delete;
  EditorCommonData& operator = (const EditorCommonData&) = delete;
  virtual ~EditorCommonData() noexcept = default;

  inline EditorScene* scene() const noexcept;
  /// Returns old scene
  virtual std::unique_ptr<EditorScene> setScene(
                                        std::unique_ptr<EditorScene> newScene);

  inline AnimationPlayer& animationPlayer() noexcept;
  inline const AnimationPlayer& animationPlayer() const noexcept;

  inline mtt::RenderScene& renderScene() noexcept;
  inline const mtt::RenderScene& renderScene() const noexcept;

signals:
  void sceneChanged(EditorScene* newScene);

private:
  AnimationPlayer _animationPlayer;
  EditorUndoStack _undoStack;
  mtt::RenderScene _renderScene;
};

inline EditorScene* EditorCommonData::scene() const noexcept
{
  return static_cast<EditorScene*>(CommonEditData::scene());
}

inline AnimationPlayer& EditorCommonData::animationPlayer() noexcept
{
  return _animationPlayer;
}

inline const AnimationPlayer& EditorCommonData::animationPlayer() const noexcept
{
  return _animationPlayer;
}

inline mtt::RenderScene& EditorCommonData::renderScene() noexcept
{
  return _renderScene;
}

inline const mtt::RenderScene& EditorCommonData::renderScene() const noexcept
{
  return _renderScene;
}
