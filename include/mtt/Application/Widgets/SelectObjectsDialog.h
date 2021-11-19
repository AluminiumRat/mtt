#pragma once

#include <functional>
#include <vector>

#include <QtCore/QSortFilterProxyModel>
#include <QtWidgets/QDialog>
#include <QtWidgets/QListView>

#include <mtt/Application/Scene/ObjectItemListModel.h>

namespace Ui
{
  class SelectObjectsDialog;
}

namespace mtt
{
  class Object;

  class SelectObjectsDialog : public QDialog
  {
    Q_OBJECT

  public:
    SelectObjectsDialog(Object& selectArea,
                        bool multiselect,
                        std::function<bool(Object&)>&& filterPredicate,
                        QWidget* parent);
    SelectObjectsDialog(const SelectObjectsDialog&) = delete;
    SelectObjectsDialog& operator = (const SelectObjectsDialog&) = delete;
    virtual ~SelectObjectsDialog() noexcept;

    inline const std::vector<Object*>& selected() const noexcept;

  private:
    void _updateSelected() noexcept;
    void _onCancel() noexcept;

  private:
    class FilterSortModel : public QSortFilterProxyModel
    {
    public:
      FilterSortModel(std::function<bool(Object&)>&& filterPredicate);

    protected:
      virtual bool filterAcceptsRow(
                              int sourceRow,
                              const QModelIndex& sourceParent) const override;
    private:
      std::function<bool(Object&)> _filterPredicate;
    };
  
  private:
    Ui::SelectObjectsDialog* _ui;

    ObjectItemListModel _itemModel;
    FilterSortModel _sortFilterModel;

    std::vector<Object*> _selected;
  };

  inline const std::vector<Object*>&
                                  SelectObjectsDialog::selected() const noexcept
  {
    return _selected;
  }
}