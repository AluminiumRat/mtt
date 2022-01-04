#pragma once

#include <QtWidgets/QStyledItemDelegate.h>

#include <mtt/Application/Widgets/ObjectsTreeView.h>

class SceneTreeView : public mtt::ObjectsTreeView
{
  Q_OBJECT

public:
  explicit SceneTreeView(mtt::CommonEditData* commonEditData);
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