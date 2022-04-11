#include <stdexcept>

#include <mtt/application/Widgets/ObjectsTreeView.h>
#include <mtt/editorLib/SceneTreeWidget/SceneTreeWidget.h>
#include <mtt/editorLib/EditorCommonData.h>
#include <mtt/utilities/Log.h>

#include <GeneratedFiles/ui_SceneTreeWidget.h>

using namespace mtt;

SceneTreeWidget::SceneTreeWidget(EditorCommonData& commonData) :
  _ui(new Ui::SceneTreeWidget),
  _commonData(commonData),
  _dataTreeView(nullptr),
  _environmentTreeView(nullptr)
{
  _ui->setupUi(this);

  std::unique_ptr<SceneTreeView> dataTreeView(new SceneTreeView(&_commonData));
  _dataTreeView = dataTreeView.get();
  _ui->sceneTreePlace->layout()->addWidget(dataTreeView.release());
  _dataTreeView->setSortFilterModel(&_dataSortModel);
  _dataTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
  _dataTreeView->setDragEnabled(true);
  _dataTreeView->setDragDropOverwriteMode(false);
  _dataTreeView->setDragDropMode(QAbstractItemView::InternalMove);
  _dataTreeView->setDefaultDropAction(Qt::MoveAction);

  std::unique_ptr<SceneTreeView> environmentTreeView(
                                              new SceneTreeView(&_commonData));
  _environmentTreeView = environmentTreeView.get();
  _ui->environmentTreePlace->layout()->addWidget(environmentTreeView.release());
  _environmentTreeView->setSortFilterModel(&_environmentSortModel);
  _environmentTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
  _environmentTreeView->setDragEnabled(true);
  _environmentTreeView->setDragDropOverwriteMode(false);
  _environmentTreeView->setDragDropMode(QAbstractItemView::InternalMove);
  _environmentTreeView->setDefaultDropAction(Qt::MoveAction);

  connect(&_commonData,
          &EditorCommonData::sceneChanged,
          this,
          &SceneTreeWidget::_setScene,
          Qt::DirectConnection);
  _setScene(_commonData.scene());
}

SceneTreeWidget::~SceneTreeWidget() noexcept = default;

void SceneTreeWidget::_resetViews() noexcept
{
  _dataTreeView->setItemModel(nullptr);
  _dataItemModel.reset();
  _environmentTreeView->setItemModel(nullptr);
  _environmentItemModel.reset();
}

void SceneTreeWidget::_setRootObjects(Object* dataRoot, Object* environmentRoot)
{
  _resetViews();

  if (dataRoot != nullptr)
  {
    _dataItemModel.emplace(*dataRoot, &_commonData.undoStack());
    _dataTreeView->setItemModel(&_dataItemModel.value());
  }

  if (environmentRoot != nullptr)
  {
    _environmentItemModel.emplace(*environmentRoot, &_commonData.undoStack());
    _environmentTreeView->setItemModel(&_environmentItemModel.value());
  }
}

void SceneTreeWidget::_setScene(EditorScene* scene) noexcept
{
  try
  {
    if (scene == nullptr) _resetViews();
    else _setRootObjects(&scene->dataRoot(), &scene->environmentRoot());
  }
  catch (std::exception& error)
  {
    _resetViews();
    Log() << "SceneTreeWidget::_setScene: unable to update widgets : " << error.what();
  }
  catch (...)
  {
    _resetViews();
    Log() << "SceneTreeWidget::_setScene: unable to update widgets.";
  }
}
