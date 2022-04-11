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
  class SceneTab;
}

namespace mtt
{
  class EditorCommonData;
  class EditorScene;

  class SceneTab : public QWidget
  {
    Q_OBJECT

  public:
    explicit SceneTab(EditorCommonData& commonData);
    SceneTab(const SceneTab&) = delete;
    SceneTab& operator = (const SceneTab&) = delete;
    virtual ~SceneTab() noexcept;

  protected:
    inline EditorCommonData& commonData() const noexcept;

    /// The return value should not be nullptr
    virtual std::unique_ptr<PropertiesWidgetFactory> createWidgetsFactory(
                                                    QVBoxLayout& widgetsLayout);

  private:
    void _setRootObjects(Object* dataRoot, Object* environmentRoot);
    void _setScene(EditorScene* scene);
    void _updatePropertiesWidgets() noexcept;
    void _clearPropertiesWidgets() noexcept;

  private:
    Ui::SceneTab* _ui;
    EditorCommonData& _commonData;

    SceneTreeView* _dataTreeView;
    std::optional<EditorObjectsItemModel> _dataItemModel;
    QSortFilterProxyModel _dataSortModel;

    SceneTreeView* _environmentTreeView;
    std::optional<EditorObjectsItemModel> _environmentItemModel;
    QSortFilterProxyModel _environmentSortModel;

    QVBoxLayout* _widgetsPlace;
  };

  inline EditorCommonData& SceneTab::commonData() const noexcept
  {
    return _commonData;
  }
}