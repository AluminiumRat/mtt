#include <mtt/editorLib/Objects/AnimationTrack.h>
#include <mtt/editorLib/Objects/DisplayedObject.h>
#include <mtt/editorLib/SceneTab/EditorObjectsItemModel.h>

using namespace mtt;

EditorObjectsItemModel::EditorObjectsItemModel( Object& rootObject,
                                                UndoStack* undoStack) :
  ObjectItemTreeModel(rootObject, undoStack)
{
}

QVariant EditorObjectsItemModel::data(const QModelIndex& index, int role) const
{
  if(role == BOLD_HIGLIGHT_ROLE)
  {
    Object& object = getObject(index);
    DisplayedObject* displayed = qobject_cast<DisplayedObject*>(&object);
    if(displayed != nullptr) return QVariant(displayed->visible());

    AnimationTrack* track = qobject_cast<AnimationTrack*>(&object);
    if (track != nullptr) return QVariant(track->enabled());

    return QVariant();
  }
  return ObjectItemTreeModel::data(index, role);
}

void EditorObjectsItemModel::connectToObject(ObjectDescription& description)
{
  ObjectDescription* descriptionPtr = &description;

  DisplayedObject* displayed =
                            qobject_cast<DisplayedObject*>(description.object);
  if(displayed != nullptr)
  {
    description.connections.push_back(
                                  connect(displayed,
                                          &DisplayedObject::visibleChanged,
                                          [=]()
                                          {
                                            _updateVisible( *descriptionPtr,
                                                            *displayed);
                                          }));
  }

  AnimationTrack* track = qobject_cast<AnimationTrack*>(description.object);
  if(track != nullptr)
  {
    description.connections.push_back(
                                  connect(track,
                                          &AnimationTrack::enabledChanged,
                                          [=]()
                                          {
                                            _updateEnabled( *descriptionPtr,
                                                            *track);
                                          }));
  }

  ObjectItemTreeModel::connectToObject(description);
}

void EditorObjectsItemModel::_updateVisible(ObjectDescription& description,
                                            DisplayedObject& object) noexcept
{
  QModelIndex index = buildIndex(description);
  emit dataChanged(index, index, { BOLD_HIGLIGHT_ROLE });
}

void EditorObjectsItemModel::_updateEnabled(ObjectDescription& description,
                                            AnimationTrack& object) noexcept
{
  QModelIndex index = buildIndex(description);
  emit dataChanged(index, index, { BOLD_HIGLIGHT_ROLE });
}
