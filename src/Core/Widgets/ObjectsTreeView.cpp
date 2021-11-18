#include <QtCore/QSortFilterProxyModel>

#include <mtt/Core/Widgets/ObjectsTreeView.h>
#include <mtt/Core/CommonEditData.h>
#include <mtt/Utilities/ScopedSetter.h>

using namespace mtt;

ObjectsTreeView::ObjectsTreeView(CommonEditData* commonEditData) :
  _itemModel(nullptr),
  _sortFilterModel(nullptr),
  _commonEditData(commonEditData),
  _skipUpdate(false)
{
  setHeaderHidden(true);

  if(commonEditData != nullptr)
  {
    connect(commonEditData,
            &CommonEditData::selectedObjectsChanged,
            this,
            &ObjectsTreeView::_updateWidgetFromSelection,
            Qt::DirectConnection);
 }
}
  
void ObjectsTreeView::setItemModel(ObjectItemTreeModel* itemModel)
{
  _itemModel = itemModel;
  if(_sortFilterModel != nullptr)
  {
    _sortFilterModel->setSourceModel(_itemModel);
    _sortFilterModel->sort(0, Qt::AscendingOrder);
  }
  else
  {
    setModel(_itemModel);
    _connectToSelectionModel();
  }
  _updateWidgetFromSelection();
}

void ObjectsTreeView::setSortFilterModel(QSortFilterProxyModel* newModel)
{
  if(_sortFilterModel != nullptr) _sortFilterModel->setSourceModel(nullptr);

  _sortFilterModel = newModel;

  if (_sortFilterModel != nullptr)
  {
    _sortFilterModel->setFilterKeyColumn(0);
    _sortFilterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    _sortFilterModel->setRecursiveFilteringEnabled(true);

    _sortFilterModel->setSourceModel(_itemModel);
    _sortFilterModel->sort(0, Qt::AscendingOrder);

    setModel(_sortFilterModel);
    _connectToSelectionModel();
  }
  else
  {
    setModel(_itemModel);
    _connectToSelectionModel();
  }
  _updateWidgetFromSelection();
}

void ObjectsTreeView::_connectToSelectionModel()
{
  connect(selectionModel(),
          &QItemSelectionModel::selectionChanged,
          this,
          &ObjectsTreeView::_updateSelectionFromWidget,
          Qt::DirectConnection);
}

void ObjectsTreeView::_updateSelectionFromWidget() noexcept
{
  if(_commonEditData == nullptr) return;
  if(_itemModel == nullptr) return;

  if(_skipUpdate) return;
  ScopedTrueSetter skipper(_skipUpdate);

  std::vector<Object*> newSelectedObjects;
  QModelIndexList selection = selectionModel()->selectedIndexes();
  for (const QModelIndex& index : selection)
  {
    if(_sortFilterModel)
    {
      newSelectedObjects.push_back(&_itemModel->getObject(
                                        _sortFilterModel->mapToSource(index)));
    }
    else
    {
      newSelectedObjects.push_back(&_itemModel->getObject(index));
    }
  }
  _commonEditData->selectObjects(newSelectedObjects);
}

void ObjectsTreeView::_updateWidgetFromSelection() noexcept
{
  if (_commonEditData == nullptr) return;
  if (_itemModel == nullptr) return;

  if (_skipUpdate) return;
  ScopedTrueSetter skipper(_skipUpdate);

  QItemSelection selection;

  std::vector<Object*> selectedObjects;
  switch(selectionMode())
  {
    case NoSelection:
      break;
    case SingleSelection:
      if(!_commonEditData->selectedObjects().empty())
      {
        selectedObjects = {_commonEditData->selectedObjects()[0]};
      }
      break;
    default:
      selectedObjects = _commonEditData->selectedObjects();
      break;
  }

  for(Object* object : selectedObjects)
  {
    QModelIndex index = _itemModel->getIndex(*object);
    if(_sortFilterModel != nullptr)
    {
      index = _sortFilterModel->mapFromSource(index);
    }
    selection.select(index, index);
    selectionModel()->setCurrentIndex(index,
                                      QItemSelectionModel::NoUpdate);
  }

  selectionModel()->select(selection, QItemSelectionModel::ClearAndSelect);
}
