#pragma once

#include <QtWidgets/QStyledItemDelegate.h>

#include <mtt/application/Widgets/ObjectsTreeView.h>

namespace mtt
{
  class SceneTreeView : public ObjectsTreeView
  {
    Q_OBJECT

  public:
    explicit SceneTreeView(CommonEditData* commonEditData);
    SceneTreeView(const SceneTreeView&) = delete;
    SceneTreeView& operator = (const SceneTreeView&) = delete;
    virtual ~SceneTreeView() noexcept = default;

  private:
    struct ItemDelegate : public QStyledItemDelegate
    {
      virtual void paint( QPainter* painter,
                          const QStyleOptionViewItem& option,
                          const QModelIndex& index) const override;
    };
    ItemDelegate _itemDelegate;
  };
}