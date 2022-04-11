#pragma once

#include <mtt/application/Scene/ObjectItemTreeModel.h>

class RootObject;

namespace mtt
{
  class AnimationTrack;
  class DisplayedObject;

  class EditorObjectsItemModel : public ObjectItemTreeModel
  {
    Q_OBJECT

  public:
    enum DataRole
    {
      BOLD_HIGLIGHT_ROLE = Qt::UserRole
    };

  public:
    EditorObjectsItemModel(Object& rootObject, UndoStack* undoStack);
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
  };
}