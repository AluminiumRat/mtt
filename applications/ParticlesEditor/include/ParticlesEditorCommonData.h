#pragma once

#include <QString>

#include <mtt/render/RenderScene.h>
#include <mtt/editorLib/AnimationPlayer.h>
#include <mtt/editorLib/EditorCommonData.h>
#include <mtt/editorLib/EditorUndoStack.h>

#include <ParticlesEditorScene.h>

class ParticlesEditorCommonData : public mtt::EditorCommonData
{
  Q_OBJECT

public:
  ParticlesEditorCommonData();
  ParticlesEditorCommonData(const ParticlesEditorCommonData&) = delete;
  ParticlesEditorCommonData& operator = (
                                    const ParticlesEditorCommonData&) = delete;
  virtual ~ParticlesEditorCommonData() noexcept = default;

  inline ParticlesEditorScene* scene() const noexcept;
  /// Returns old scene
  virtual std::unique_ptr<ParticlesEditorScene> setScene(
                                std::unique_ptr<ParticlesEditorScene> newScene);

  inline mtt::AnimationPlayer& animationPlayer() noexcept;
  inline const mtt::AnimationPlayer& animationPlayer() const noexcept;

signals:
  void sceneChanged(ParticlesEditorScene* newScene);

private:
  mtt::AnimationPlayer _animationPlayer;
  mtt::EditorUndoStack _undoStack;
};

inline ParticlesEditorScene* ParticlesEditorCommonData::scene() const noexcept
{
  return static_cast<ParticlesEditorScene*>(CommonEditData::scene());
}

inline mtt::AnimationPlayer&
                          ParticlesEditorCommonData::animationPlayer() noexcept
{
  return _animationPlayer;
}

inline const mtt::AnimationPlayer&
                    ParticlesEditorCommonData::animationPlayer() const noexcept
{
  return _animationPlayer;
}
