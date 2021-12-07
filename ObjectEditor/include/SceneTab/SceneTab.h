#pragma once

#include <optional>

#include <QtCore/QSortFilterProxyModel>
#include <QtWidgets/QWidget>

#include <SceneTab/EditorObjectsItemModel.h>
#include <SceneTab/SceneTreeView.h>

class EditorCommonData;
class EditorScene;
class QVBoxLayout;

namespace Ui
{
  class sceneTab;
}

class SceneTab : public QWidget
{
  Q_OBJECT

public:
  explicit SceneTab(EditorCommonData& commonEditData);
  SceneTab(const SceneTab&) = delete;
  SceneTab& operator = (const SceneTab&) = delete;
  virtual ~SceneTab() noexcept;

private:
  void _setScene(EditorScene* scene);
  void _updatePropertiesWidgets();

private:
  Ui::sceneTab* _ui;
  EditorCommonData& _commonEditData;
  SceneTreeView* _treeView;
  std::optional<EditorObjectsItemModel> _itemModel;
  QSortFilterProxyModel _sortModel;

  QVBoxLayout* _widgetsPlace;
};