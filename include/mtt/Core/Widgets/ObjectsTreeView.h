#pragma once

#include <QtWidgets/QTreeView>

#include <mtt/Core/Scene/ObjectItemTreeModel.h>

class QSortFilterProxyModel;

namespace mtt
{
  class CommonEditData;

  class ObjectsTreeView : public QTreeView
  {
    Q_OBJECT
  
  public:
    explicit ObjectsTreeView(CommonEditData* commonEditData);
    ObjectsTreeView(const ObjectsTreeView&) = delete;
    ObjectsTreeView& operator = (const ObjectsTreeView&) = delete;
    virtual ~ObjectsTreeView() noexcept = default;

    inline CommonEditData* commonEditData() const noexcept;

    inline ObjectItemTreeModel* itemModel() const noexcept;
    void setItemModel(ObjectItemTreeModel* itemModel);

    inline QSortFilterProxyModel* sortFilterModel() const noexcept;
    void setSortFilterModel(QSortFilterProxyModel* newModel);

  private:
    void _updateSelectionFromWidget() noexcept;
    void _updateWidgetFromSelection() noexcept;
    void _connectToSelectionModel();

  private:
    ObjectItemTreeModel* _itemModel;
    QSortFilterProxyModel* _sortFilterModel;
    CommonEditData* _commonEditData;
    bool _skipUpdate;
  };

  inline CommonEditData* ObjectsTreeView::commonEditData() const noexcept
  {
    return _commonEditData;
  }

  inline ObjectItemTreeModel* ObjectsTreeView::itemModel() const noexcept
  {
    return _itemModel;
  }

  inline QSortFilterProxyModel*
                              ObjectsTreeView::sortFilterModel() const noexcept
  {
    return _sortFilterModel;
  }
}