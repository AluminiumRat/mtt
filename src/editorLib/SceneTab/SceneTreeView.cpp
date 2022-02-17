#include <QtWidgets/QApplication>

#include <mtt/editorLib/SceneTab/EditorObjectsItemModel.h>
#include <mtt/editorLib/SceneTab/SceneTreeView.h>

using namespace mtt;

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

SceneTreeView::SceneTreeView(CommonEditData* commonEditData) :
  ObjectsTreeView(commonEditData)
{
  setItemDelegate(&_itemDelegate);
}
