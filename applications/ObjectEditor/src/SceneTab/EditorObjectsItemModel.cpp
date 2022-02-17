#include <mtt/editorLib/Objects/AnimationTrack.h>
#include <mtt/editorLib/Objects/DisplayedObject.h>

#include <Objects/RootObject.h>
#include <SceneTab/EditorObjectsItemModel.h>

EditorObjectsItemModel::EditorObjectsItemModel( RootObject& rootObject,
                                                mtt::UndoStack* undoStack) :
  ObjectItemTreeModel(rootObject, undoStack),
  _root(rootObject)
{
}

QVariant EditorObjectsItemModel::data(const QModelIndex& index, int role) const
{
  if(role == BOLD_HIGLIGHT_ROLE)
  {
    mtt::Object& object = getObject(index);
    mtt::DisplayedObject* displayed =
                                  qobject_cast<mtt::DisplayedObject*>(&object);
    if(displayed != nullptr) return QVariant(displayed->visible());

    mtt::AnimationTrack* track = qobject_cast<mtt::AnimationTrack*>(&object);
    if (track != nullptr) return QVariant(track->enabled());

    return QVariant();
  }
  return ObjectItemTreeModel::data(index, role);
}

void EditorObjectsItemModel::connectToObject(ObjectDescription& description)
{
  ObjectDescription* descriptionPtr = &description;

  mtt::DisplayedObject* displayed =
                        qobject_cast<mtt::DisplayedObject*>(description.object);
  if(displayed != nullptr)
  {
    description.connections.push_back(
                                  connect(displayed,
                                          &mtt::DisplayedObject::visibleChanged,
                                          [=]()
                                          {
                                            _updateVisible( *descriptionPtr,
                                                            *displayed);
                                          }));
  }

  mtt::AnimationTrack* track =
                        qobject_cast<mtt::AnimationTrack*>(description.object);
  if(track != nullptr)
  {
    description.connections.push_back(
                                  connect(track,
                                          &mtt::AnimationTrack::enabledChanged,
                                          [=]()
                                          {
                                            _updateEnabled( *descriptionPtr,
                                                            *track);
                                          }));
  }

  ObjectItemTreeModel::connectToObject(description);
}

void EditorObjectsItemModel::_updateVisible(
                                          ObjectDescription& description,
                                          mtt::DisplayedObject& object) noexcept
{
  QModelIndex index = buildIndex(description);
  emit dataChanged(index, index, { BOLD_HIGLIGHT_ROLE });
}

void EditorObjectsItemModel::_updateEnabled(
                                          ObjectDescription& description,
                                          mtt::AnimationTrack& object) noexcept
{
  QModelIndex index = buildIndex(description);
  emit dataChanged(index, index, { BOLD_HIGLIGHT_ROLE });
}
