#pragma once

#include <memory>
#include <optional>

#include <QtCore/QSortFilterProxyModel>
#include <QtWidgets/QWidget>

#include <mtt/editorLib/SceneTreeWidget/EditorObjectsItemModel.h>
#include <mtt/editorLib/SceneTreeWidget/SceneTreeView.h>

class QVBoxLayout;

namespace Ui
{
  class SceneTreeWidget;
}

namespace mtt
{
  class EditorCommonData;
  class EditorScene;

  class SceneTreeWidget : public QWidget
  {
    Q_OBJECT

  public:
    explicit SceneTreeWidget(EditorCommonData& commonData);
    SceneTreeWidget(const SceneTreeWidget&) = delete;
    SceneTreeWidget& operator = (const SceneTreeWidget&) = delete;
    virtual ~SceneTreeWidget() noexcept;

    inline EditorCommonData& commonData() const noexcept;

  private:
    void _resetViews() noexcept;
    void _setRootObjects(Object* dataRoot, Object* environmentRoot);
    void _setScene(EditorScene* scene) noexcept;

  private:
    std::unique_ptr<Ui::SceneTreeWidget> _ui;
    EditorCommonData& _commonData;

    SceneTreeView* _dataTreeView;
    std::optional<EditorObjectsItemModel> _dataItemModel;
    QSortFilterProxyModel _dataSortModel;

    SceneTreeView* _environmentTreeView;
    std::optional<EditorObjectsItemModel> _environmentItemModel;
    QSortFilterProxyModel _environmentSortModel;
  };

  inline EditorCommonData& SceneTreeWidget::commonData() const noexcept
  {
    return _commonData;
  }
}