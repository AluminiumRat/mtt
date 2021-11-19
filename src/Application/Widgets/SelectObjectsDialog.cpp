#include <memory>
#include <stdexcept>

#include <mtt/Application/Widgets/SelectObjectsDialog.h>
#include <mtt/Utilities/Abort.h>

#include <GeneratedFiles/ui_SelectObjectsDialog.h>

using namespace mtt;

SelectObjectsDialog::FilterSortModel::FilterSortModel(
                              std::function<bool(Object&)>&& filterPredicate) :
  _filterPredicate(filterPredicate)
{
}

bool SelectObjectsDialog::FilterSortModel::filterAcceptsRow(
                                          int sourceRow,
                                          const QModelIndex& sourceParent) const
{
  try
  {
    bool defaultResult = QSortFilterProxyModel::filterAcceptsRow( sourceRow,
                                                                sourceParent);
    if (defaultResult == false) return false;

    if(!_filterPredicate) return true;

    ObjectItemListModel* model = static_cast<ObjectItemListModel*>(sourceModel());
    QModelIndex index = model->index(sourceRow, 0, sourceParent);
    Object& object = model->getObject(index);
  
    return _filterPredicate(object);
  }
  catch(...)
  {
    return false;
  }
}

SelectObjectsDialog::SelectObjectsDialog(
                                Object& selectArea,
                                bool multiselect,
                                std::function<bool(Object&)>&& filterPredicate,
                                QWidget* parent) :
  QDialog(parent),
  _ui(new Ui::SelectObjectsDialog),
  _itemModel(selectArea),
  _sortFilterModel(std::move(filterPredicate))
{
  setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
  _ui->setupUi(this);

  _sortFilterModel.setSourceModel(&_itemModel);
  _sortFilterModel.sort(0);
  _ui->listView->setModel(&_sortFilterModel);

  if(multiselect)
  {
    _ui->listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
  }

  connect(_ui->okButton,
          &QPushButton::pressed,
          this,
          &SelectObjectsDialog::accept,
          Qt::DirectConnection);

  connect(_ui->cancelButton,
          &QPushButton::pressed,
          this,
          &SelectObjectsDialog::_onCancel,
          Qt::DirectConnection);

  connect(_ui->listView->selectionModel(),
          &QItemSelectionModel::selectionChanged,
          this,
          &SelectObjectsDialog::_updateSelected,
          Qt::DirectConnection);
}

SelectObjectsDialog::~SelectObjectsDialog() noexcept
{
  delete _ui;
}

void SelectObjectsDialog::_updateSelected() noexcept
{
  QModelIndexList selection =
                            _ui->listView->selectionModel()->selectedIndexes();
  _ui->okButton->setEnabled(!selection.empty());
  _selected.clear();
  try
  {
    for (const QModelIndex& index : selection)
    {
      _selected.push_back(&_itemModel.getObject(
                                          _sortFilterModel.mapToSource(index)));
    }
  }
  catch(std::exception& error)
  {
    Log() << "SelectObjectsDialog::_updateSelected: " << error.what();
    Abort("SelectObjectsDialog::_updateSelected: unable to get selected objects.");
  }
  catch(...)
  {
    Abort("SelectObjectsDialog::_updateSelected: unable to get selected objects.");
  }
}

void SelectObjectsDialog::_onCancel() noexcept
{
  _selected.clear();
  reject();
}
