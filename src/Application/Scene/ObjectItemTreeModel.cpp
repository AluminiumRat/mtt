#include <memory>

#include <QtCore/QDataStream>
#include <QtCore/QMimeData>

#include <mtt/Application/EditCommands/CompositeCommand.h>
#include <mtt/Application/EditCommands/MoveObjectCommand.h>
#include <mtt/Application/EditCommands/UndoStack.h>
#include <mtt/Application/Scene/ObjectItemTreeModel.h>
#include <mtt/Application/Scene/Object.h>
#include <mtt/Utilities/Abort.h>

using namespace mtt;

ObjectItemTreeModel::ObjectItemTreeModel( Object& rootObject,
                                          UndoStack* undoStack) :
  _rootObject(rootObject),
  _undoStack(undoStack)
{
  _rootDescription = std::move(_buildDescription(_rootObject, nullptr));
}

ObjectItemTreeModel::~ObjectItemTreeModel() noexcept
{
}

std::unique_ptr<ObjectItemTreeModel::ObjectDescription>
                              ObjectItemTreeModel::_buildDescription(
                                                      Object& object,
                                                      ObjectDescription* parent)
{
  std::unique_ptr<ObjectDescription> description( new ObjectDescription(object,
                                                                        parent,
                                                                        *this));

  for(size_t childIndex = 0;
      childIndex < object.subobjectNumber();
      childIndex++)
  {
    description->childs.push_back(std::move(_buildDescription(
                                            object.subobject(childIndex),
                                            description.get())));
  }

  connectToObject(*description);
  return description;
}

void ObjectItemTreeModel::connectToObject(ObjectDescription& description)
{
  ObjectDescription* descriptionPtr = &description;

  description.connections.push_back(connect(description.object,
                                            &Object::nameChanged,
                                            [=]()
                                            {
                                              _updateName(*descriptionPtr);
                                            }));

  description.connections.push_back(connect(description.object,
                                            &Object::subobjectAdded,
                                            [=](Object& child)
                                            {
                                              _onChildAdded(*descriptionPtr,
                                                            child);
                                            }));

  description.connections.push_back(connect(description.object,
                                            &Object::subobjectRemoved,
                                            [=](Object& child)
                                            {
                                              _onChildRemoved(*descriptionPtr,
                                                              child);
                                            }));
}

void ObjectItemTreeModel::_updateName(ObjectDescription& description) noexcept
{
  QModelIndex index = buildIndex(description);
  emit dataChanged(index, index, { Qt::DisplayRole });
}

void ObjectItemTreeModel::_onChildAdded(ObjectDescription& groupDescription,
                                        Object& child) noexcept
{
  try
  {
    std::unique_ptr<ObjectDescription> object = _buildDescription(
                                                            child,
                                                            &groupDescription);
    QModelIndex groupIndex = buildIndex(groupDescription);
    beginInsertRows(groupIndex,
                    int(groupDescription.childs.size()),
                    int(groupDescription.childs.size()));
    groupDescription.childs.push_back(std::move(object));
    endInsertRows();
  }
  catch(...)
  {
    Abort("ObjectItemTreeModel::onChildAdded: unable to add row to item model.");
  }
}

void ObjectItemTreeModel::_onChildRemoved(ObjectDescription& groupDescription,
                                          Object& child) noexcept
{
  try
  {
    for(int childIndex = 0;
        childIndex < groupDescription.childs.size();
        childIndex++)
    {
      if(groupDescription.childs[childIndex]->object == &child)
      {
        QModelIndex groupIndex = buildIndex(groupDescription);
        beginRemoveRows(groupIndex, childIndex, childIndex);
        groupDescription.childs.erase(
                                  groupDescription.childs.begin() + childIndex);
        endRemoveRows();
        return;
      }
    }
    Abort("ObjectItemTreeModel::onChildRemoved: child description is not found.");
  }
  catch (...)
  {
    Abort("ObjectItemTreeModel::onChildRemoved: unable to remove rows from item model.");
  }
}

QModelIndex ObjectItemTreeModel::buildIndex(
                                  ObjectDescription& description) const noexcept
{
  ObjectDescription* parent = description.parent;
  if(parent == nullptr) return QModelIndex();
  else
  {
    for(int childIndex = 0; childIndex < parent->childs.size(); childIndex++)
    {
      if(parent->childs[childIndex].get() == &description)
      {
        return createIndex(childIndex, 0, &description);
      }
    }
  }
  Abort("ObjectItemTreeModel::buildIndex: child description is not found.");
}

ObjectItemTreeModel::ObjectDescription&
                        ObjectItemTreeModel::getDescription(
                                        const QModelIndex& index) const noexcept
{
  if (!index.isValid()) return *_rootDescription;
  if (index.internalPointer() == nullptr) Abort("ObjectItemTreeModel::getDescription: internal pointer is null.");
  return *static_cast<ObjectDescription*>(index.internalPointer());
}

Object& ObjectItemTreeModel::getObject(const QModelIndex& index) const noexcept
{
  ObjectDescription& description = getDescription(index);
  return *description.object;
}

QVariant ObjectItemTreeModel::data(const QModelIndex& index, int role) const
{
  Object& object = getObject(index);
  return data(object, role);
}

QVariant ObjectItemTreeModel::data(const Object& object, int role) const
{
  if(role == Qt::DisplayRole) return object.name();
  return QVariant();
}

QVariant ObjectItemTreeModel::headerData( int section,
                                          Qt::Orientation orientation,
                                          int role) const
{
  return QVariant();
}

QModelIndex ObjectItemTreeModel::index( int row,
                                        int column,
                                        const QModelIndex& parent) const
{
  if (!hasIndex(row, column, parent)) return QModelIndex();
  if (column != 0) return QModelIndex();

  ObjectDescription& parentObject = getDescription(parent);
  return createIndex( row,
                      column,
                      parentObject.childs[row].get());
}

QModelIndex ObjectItemTreeModel::parent(const QModelIndex& index) const
{
  ObjectDescription& object = getDescription(index);

  ObjectDescription* parent = object.parent;
  if (object.parent == nullptr) return QModelIndex();

  ObjectDescription* super = parent->parent;
  if(super == nullptr) return QModelIndex();

  for(int childIndex = 0;
      childIndex < super->childs.size();
      childIndex++)
  {
    if(parent == super->childs[childIndex].get())
    {
      return createIndex(childIndex, 0, parent);
    }
  }

  Abort("ObjectItemTreeModel::parent: parent object not found.");
}

int ObjectItemTreeModel::rowCount(const QModelIndex& parent) const
{
  ObjectDescription& object = getDescription(parent);
  return int(object.childs.size());
}

QModelIndex ObjectItemTreeModel::getIndex(const Object& object) const noexcept
{
  DescriptionsMap::const_iterator iDescrition =
                                            _descriptionsMap.find(object.id());
  if(iDescrition == _descriptionsMap.end()) Abort("ObjectItemTreeModel::getIndex: objects description is not found.");

  ObjectDescription* description = iDescrition->second;
  if(description == _rootDescription.get()) return QModelIndex();

  ObjectDescription* parent = description->parent;
  if(parent == nullptr) Abort("ObjectItemTreeModel::getIndex: description has no parent.");
  for(int childIndex = 0; childIndex < parent->childs.size(); childIndex++)
  {
    if(parent->childs[childIndex]->object == &object)
    {
      return createIndex(childIndex, 0, description);
    }
  }
  Abort("ObjectItemTreeModel::getIndex: the child node is not found.");
}

int ObjectItemTreeModel::columnCount(const QModelIndex& parent) const
{
  return 1;
}

Qt::DropActions ObjectItemTreeModel::supportedDropActions() const
{
  return Qt::MoveAction;
}

Qt::ItemFlags ObjectItemTreeModel::flags(const QModelIndex& index) const
{
  Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);
  if (!index.isValid()) return defaultFlags;

  Object& object = getObject(index);
  if (object.parent() == nullptr) return defaultFlags;

  if (object.parent()->subobjectCanBeAddedAndRemoved(object))
  {
    return Qt::ItemIsDropEnabled | Qt::ItemIsDragEnabled | defaultFlags;
  }
  return Qt::ItemIsDropEnabled | defaultFlags;
}

QStringList ObjectItemTreeModel::mimeTypes() const
{
  QStringList types;
  types << "application/vnd.uid.list";
  return types;
}

QMimeData* ObjectItemTreeModel::mimeData(const QModelIndexList& indexes) const
{
  std::unique_ptr<QMimeData> mimeData(new QMimeData());
  QByteArray encodedData;

  QDataStream stream(&encodedData, QIODevice::WriteOnly);

  for (const QModelIndex& index : indexes)
  {
    Object& object = getObject(index);
    stream << QString::number(object.id().value());
  }

  mimeData->setData("application/vnd.uid.list", encodedData);
  return mimeData.release();
}

bool ObjectItemTreeModel::dropMimeData( const QMimeData* data,
                                        Qt::DropAction action,
                                        int row,
                                        int column,
                                        const QModelIndex& parent)
{
  if (action == Qt::IgnoreAction) return true;
  if (action != Qt::MoveAction) return false;

  Object& targetGroup = getObject(parent);

  QByteArray encodedData = data->data("application/vnd.uid.list");
  QDataStream stream(&encodedData, QIODevice::ReadOnly);

  std::set<Object*> movedObjects;
  while (!stream.atEnd())
  {
    QString textId;
    stream >> textId;
    UID objectId(textId.toULongLong());

    DescriptionsMap::const_iterator iDescrition =
                                                _descriptionsMap.find(objectId);
    if(iDescrition == _descriptionsMap.end()) return false;
    movedObjects.insert(iDescrition->second->object);
  }

  _filterMovedObjects(movedObjects, targetGroup);
  if(movedObjects.empty()) return false;

  try
  {
    std::unique_ptr<CompositeCommand> compositeCommand;
    if(_undoStack != nullptr) compositeCommand.reset(new CompositeCommand);

    for(Object* object : movedObjects)
    {
      std::unique_ptr<MoveObjectCommand>
                        moveCommand(new MoveObjectCommand(*object, targetGroup));
      if(_undoStack != nullptr)
      {
        compositeCommand->addSubcommand(std::move(moveCommand));
      }
      else moveCommand->make();
    }

    if(_undoStack != nullptr)
    {
      _undoStack->addAndMake(std::move(compositeCommand));
    }
  }
  catch(std::exception& error)
  {
    Log() << "ObjectItemTreeModel::dropMimeData: " << error.what();
    return false;
  }
  catch(...)
  {
    Log() << "ObjectItemTreeModel::dropMimeData: unable to mave object";
    return false;
  }

  return true;
}

void ObjectItemTreeModel::_filterMovedObjects( std::set<Object*>& objects,
                                              const Object& targetGroup) const
{
  std::set<Object*>::iterator iObject = objects.begin();
  while(iObject != objects.end())
  {
    Object* object = *iObject;
    Object* parent = object->parent();

    bool needDelete = false;
    if(parent == nullptr) needDelete = true;
    else
    {
      if( !parent->subobjectCanBeAddedAndRemoved(*object) ||
          !targetGroup.subobjectCanBeAddedAndRemoved(*object))
      {
        objects.clear();
        return;
      }
    }
    
    if (parent == &targetGroup) needDelete = true;

    while(parent != nullptr)
    {
      if(objects.find(parent) != objects.end())
      {
        needDelete = true;
        break;
      }
      parent = parent->parent();
    }
    
    if(needDelete) iObject = objects.erase(iObject);
    else iObject++;
  }
}
