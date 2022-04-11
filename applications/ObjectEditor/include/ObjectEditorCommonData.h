#pragma once

#include <QString>

#include <mtt/render/RenderScene.h>
#include <mtt/editorLib/AnimationPlayer.h>
#include <mtt/editorLib/EditorCommonData.h>
#include <mtt/editorLib/EditorUndoStack.h>

#include <ObjectEditorScene.h>

class ObjectEditorCommonData : public mtt::EditorCommonData
{
  Q_OBJECT

public:
  ObjectEditorCommonData();
  ObjectEditorCommonData(const ObjectEditorCommonData&) = delete;
  ObjectEditorCommonData& operator = (const ObjectEditorCommonData&) = delete;
  virtual ~ObjectEditorCommonData() noexcept = default;

  inline ObjectEditorScene* scene() const noexcept;
  /// Returns old scene
  virtual std::unique_ptr<ObjectEditorScene> setScene(
                                    std::unique_ptr<ObjectEditorScene> newScene);

  inline mtt::AnimationPlayer& animationPlayer() noexcept;
  inline const mtt::AnimationPlayer& animationPlayer() const noexcept;

signals:
  void sceneChanged(ObjectEditorScene* newScene);

private:
  mtt::AnimationPlayer _animationPlayer;
  mtt::EditorUndoStack _undoStack;
};

inline ObjectEditorScene* ObjectEditorCommonData::scene() const noexcept
{
  return static_cast<ObjectEditorScene*>(CommonEditData::scene());
}

inline mtt::AnimationPlayer& ObjectEditorCommonData::animationPlayer() noexcept
{
  return _animationPlayer;
}

inline const mtt::AnimationPlayer&
                        ObjectEditorCommonData::animationPlayer() const noexcept
{
  return _animationPlayer;
}
