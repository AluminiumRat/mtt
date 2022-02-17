#pragma once

#include <QString>

#include <mtt/application/CommonEditData.h>
#include <mtt/render/RenderScene.h>
#include <mtt/editorLib/AnimationPlayer.h>

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

  inline mtt::AnimationPlayer& animationPlayer() noexcept;
  inline const mtt::AnimationPlayer& animationPlayer() const noexcept;

  inline mtt::RenderScene& renderScene() noexcept;
  inline const mtt::RenderScene& renderScene() const noexcept;

  inline const QString& modelFilename() const noexcept;
  void setModelFilename(const QString& newValue) noexcept;

  inline const QString& environmentFilename() const noexcept;
  void setEnvironmentFilename(const QString& newValue) noexcept;

signals:
  void sceneChanged(EditorScene* newScene);
  void modelFilenameChanged(const QString& newValue);
  void environmentFilenameChanged(const QString& newValue);

private:
  mtt::AnimationPlayer _animationPlayer;
  EditorUndoStack _undoStack;
  mtt::RenderScene _renderScene;
  QString _modelFilename;
  QString _environmentFilename;
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

inline const QString& EditorCommonData::modelFilename() const noexcept
{
  return _modelFilename;
}

inline const QString& EditorCommonData::environmentFilename() const noexcept
{
  return _environmentFilename;
}
