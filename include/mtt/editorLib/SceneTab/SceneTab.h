#pragma once

#include <memory>
#include <optional>

#include <QtCore/QSortFilterProxyModel>
#include <QtWidgets/QWidget>

#include <mtt/editorLib/SceneTab/EditorObjectsItemModel.h>
#include <mtt/editorLib/SceneTab/PropertiesWidgetFactory.h>
#include <mtt/editorLib/SceneTab/SceneTreeView.h>

class QVBoxLayout;

namespace Ui
{
  class sceneTab;
}

namespace mtt
{
  class CommonEditData;

  class SceneTab : public QWidget
  {
    Q_OBJECT

  public:
    explicit SceneTab(CommonEditData& commonEditData);
    SceneTab(const SceneTab&) = delete;
    SceneTab& operator = (const SceneTab&) = delete;
    virtual ~SceneTab() noexcept;

  protected:
    inline CommonEditData& commonEditData() const noexcept;

    void setRootObject(Object* newRoot);

    /// The return value should not be nullptr
    virtual std::unique_ptr<PropertiesWidgetFactory> createWidgetsFactory(
                                                    QVBoxLayout& widgetsLayout);

  private:
    void _updatePropertiesWidgets() noexcept;
    void _clearPropertiesWidgets() noexcept;

  private:
    Ui::sceneTab* _ui;
    CommonEditData& _commonEditData;
    SceneTreeView* _treeView;
    std::optional<EditorObjectsItemModel> _itemModel;
    QSortFilterProxyModel _sortModel;

    QVBoxLayout* _widgetsPlace;
  };

  inline CommonEditData& SceneTab::commonEditData() const noexcept
  {
    return _commonEditData;
  }
}