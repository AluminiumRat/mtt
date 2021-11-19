#pragma once

#include <mtt/Core/Scene/ObjectItemTreeModel.h>

class AnimationTrack;
class DisplayedObject;
class RootObject;

class EditorObjectsItemModel : public mtt::ObjectItemTreeModel
{
  Q_OBJECT

public:
  enum DataRole
  {
    BOLD_HIGLIGHT_ROLE = Qt::UserRole
  };

public:
  EditorObjectsItemModel(RootObject& rootObject, mtt::UndoStack* undoStack);
  EditorObjectsItemModel(const EditorObjectsItemModel&) = delete;
  EditorObjectsItemModel& operator = (const EditorObjectsItemModel&) = delete;
  virtual ~EditorObjectsItemModel() noexcept = default;

  virtual QVariant data(const QModelIndex& index, int role) const override;

protected:
  virtual void connectToObject(ObjectDescription& description);

private:
  void _updateVisible(ObjectDescription& description,
                      DisplayedObject& object) noexcept;

  void _updateEnabled(ObjectDescription& description,
                      AnimationTrack& object) noexcept;

private:
  RootObject& _root;
};
