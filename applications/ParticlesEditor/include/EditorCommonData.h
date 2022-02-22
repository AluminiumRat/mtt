#pragma once

#include <QString>

#include <mtt/application/CommonEditData.h>
#include <mtt/render/RenderScene.h>
#include <mtt/editorLib/AnimationPlayer.h>
#include <mtt/editorLib/EditorUndoStack.h>

#include <EditorScene.h>

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

  inline mtt::AnimationPlayer& animationPlayer() noexcept;
  inline const mtt::AnimationPlayer& animationPlayer() const noexcept;

  inline mtt::RenderScene& renderScene() noexcept;
  inline const mtt::RenderScene& renderScene() const noexcept;

  inline const QString& effectFilename() const noexcept;
  void setEffectFilename(const QString& newValue) noexcept;

signals:
  void sceneChanged(EditorScene* newScene);
  void effectFilenameChanged(const QString& newValue);

private:
  mtt::AnimationPlayer _animationPlayer;
  mtt::EditorUndoStack _undoStack;
  mtt::RenderScene _renderScene;
  QString _effectFilename;
};

inline EditorScene* EditorCommonData::scene() const noexcept
{
  return static_cast<EditorScene*>(CommonEditData::scene());
}

inline mtt::AnimationPlayer& EditorCommonData::animationPlayer() noexcept
{
  return _animationPlayer;
}

inline const mtt::AnimationPlayer&
                              EditorCommonData::animationPlayer() const noexcept
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

inline const QString& EditorCommonData::effectFilename() const noexcept
{
  return _effectFilename;
}
