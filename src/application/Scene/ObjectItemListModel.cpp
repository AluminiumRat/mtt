#include <mtt/application/Scene/ObjectItemListModel.h>
#include <mtt/application/Scene/Object.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

ObjectItemListModel::ObjectItemListModel(Object& rootObject) :
  _rootObject(rootObject),
  _initialized(false)
{
}

void ObjectItemListModel::_initIfNeeded() const noexcept
{
  if(!_initialized)
  {
    try
    {
      ObjectItemListModel* model = const_cast<ObjectItemListModel*>(this);
      model->_init();
    }
    catch (std::exception& error)
    {
      Log() << error.what();
      Abort("ObjectItemListModel::_initIfNeeded(): unable to initialize.");
    }
    catch (...)
    {
      Abort("ObjectItemListModel::_initIfNeeded(): unable to initialize.");
    }
  }
}

void ObjectItemListModel::_init()
{
  _initialized = true;

  connect(&_rootObject,
          &Object::subobjectAdded,
          this,
          &ObjectItemListModel::_addItems,
          Qt::DirectConnection);

  connect(&_rootObject,
          &Object::subobjectRemoved,
          this,
          &ObjectItemListModel::_removeItems,
          Qt::DirectConnection);

  for(size_t childIndex = 0;
      childIndex < _rootObject.subobjectNumber();
      childIndex++)
  {
    _addItems(_rootObject.subobject(childIndex));
  }
}

void ObjectItemListModel::connectToObject(ObjectDescription& description)
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
                                            this,
                                            &ObjectItemListModel::_addItems,
                                            Qt::DirectConnection));

  description.connections.push_back(connect(description.object,
                                            &Object::subobjectRemoved,
                                            this,
                                            &ObjectItemListModel::_removeItems,
                                            Qt::DirectConnection));
}

void ObjectItemListModel::_updateName(ObjectDescription& description) noexcept
{
  QModelIndex index = buildIndex(description);
  emit dataChanged(index, index, { Qt::DisplayRole });
}

void ObjectItemListModel::_addItems(Object& object) noexcept
{
  try
  {
    int newIndex = int(_descriptions.size());
    std::unique_ptr<ObjectDescription> description(
                                new ObjectDescription(object, newIndex, *this));
    connectToObject(*description);

    beginInsertRows(QModelIndex(),
                    int(_descriptions.size()),
                    int(_descriptions.size()));
    _descriptions.push_back(std::move(description));
    endInsertRows();
  }
  catch(...)
  {
    Abort("ObjectItemListModel::_addItems: unable to add row to item model.");
  }

  for(size_t childIndex = 0;
      childIndex < object.subobjectNumber();
      childIndex++)
  {
    _addItems(object.subobject(childIndex));
  }
}

void ObjectItemListModel::_removeItems(Object& object) noexcept
{
  DescriptionsMap::const_iterator iDescrition =
                                            _descriptionsMap.find(object.id());
  if (iDescrition == _descriptionsMap.end()) Abort("ObjectItemListModel::_removeItems: objects description is not found.");
  ObjectDescription* description = iDescrition->second;
  int descriptionIndex = description->index;

  try
  {
    beginRemoveRows(QModelIndex(), descriptionIndex, descriptionIndex);
    _descriptions.erase(_descriptions.begin() + descriptionIndex);
    for (; descriptionIndex < _descriptions.size(); descriptionIndex++)
    {
      _descriptions[descriptionIndex]->index = descriptionIndex;
    }
    endRemoveRows();
  }
  catch (...)
  {
    Abort("ObjectItemListModel::_removeItems: unable to remove rows from item model.");
  }

  for(size_t childIndex = 0;
      childIndex < object.subobjectNumber();
      childIndex++)
  {
    _removeItems(object.subobject(childIndex));
  }
}

QModelIndex ObjectItemListModel::buildIndex(
                                  ObjectDescription& description) const noexcept
{
  return createIndex(description.index, 0, &description);
}

ObjectItemListModel::ObjectDescription&
                        ObjectItemListModel::getDescription(
                                        const QModelIndex& index) const noexcept
{
  if (!index.isValid()) Abort("ObjectItemListModel::getDescription: invalid index.");
  if (index.internalPointer() == nullptr) Abort("ObjectItemListModel::getDescription: internal pointer is null.");
  return *static_cast<ObjectDescription*>(index.internalPointer());
}

Object& ObjectItemListModel::getObject(const QModelIndex& index) const noexcept
{
  ObjectDescription& description = getDescription(index);
  return *description.object;
}

QVariant ObjectItemListModel::data(const QModelIndex& index, int role) const
{
  Object& object = getObject(index);
  return data(object, role);
}

QVariant ObjectItemListModel::headerData( int section,
                                          Qt::Orientation orientation,
                                          int role) const
{
  return QVariant();
}

QVariant ObjectItemListModel::data(const Object& object, int role) const
{
  if(role == Qt::DisplayRole) return object.name();
  return QVariant();
}

QModelIndex ObjectItemListModel::index( int row,
                                        int column,
                                        const QModelIndex& parent) const
{
  _initIfNeeded();
  if(!hasIndex(row, column, parent)) return QModelIndex();
  if(column != 0) return QModelIndex();
  if(parent.isValid()) return QModelIndex();

  return createIndex( row,
                      column,
                      _descriptions[row].get());
}

QModelIndex ObjectItemListModel::parent(const QModelIndex& index) const
{
  return QModelIndex();
}

int ObjectItemListModel::rowCount(const QModelIndex& parent) const
{
  _initIfNeeded();
  if(parent.isValid()) return 0;
  else return int(_descriptions.size());
}

QModelIndex ObjectItemListModel::getIndex(const Object& object) const noexcept
{
  _initIfNeeded();
  DescriptionsMap::const_iterator iDescrition =
                                            _descriptionsMap.find(object.id());
  if(iDescrition == _descriptionsMap.end()) Abort("ObjectItemListModel::getIndex: objects description is not found.");

  ObjectDescription* description = iDescrition->second;
  return createIndex(description->index, 0, description);
}

int ObjectItemListModel::columnCount(const QModelIndex& parent) const
{
  return 1;
}
