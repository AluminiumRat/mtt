#include <QtWidgets/QApplication>

#include <SceneTab/EditorObjectsItemModel.h>
#include <SceneTab/SceneTreeView.h>

void SceneTreeView::ItemDelegate::paint(QPainter* painter,
                                        const QStyleOptionViewItem& option,
                                        const QModelIndex& index) const
{
  QStyleOptionViewItem modifiedOption = option;
  initStyleOption(&modifiedOption, index);

  QVariant visible = index.data(EditorObjectsItemModel::BOLD_HIGLIGHT_ROLE);
  if(visible.toBool()) modifiedOption.font.setBold(true);

  QApplication::style()->drawControl( QStyle::CE_ItemViewItem,
                                      &modifiedOption,
                                      painter);
}

SceneTreeView::SceneTreeView(mtt::CommonEditData* commonEditData) :
  ObjectsTreeView(commonEditData)
{
  setItemDelegate(&_itemDelegate);
}
