#include <stdexcept>

#include <QtWidgets/QVBoxLayout>

#include <mtt/application/Widgets/ObjectsTreeView.h>
#include <mtt/editorLib/SceneTab/SceneTab.h>
#include <mtt/editorLib/EditorCommonData.h>
#include <mtt/utilities/Abort.h>

#include <GeneratedFiles/ui_SceneTab.h>

using namespace mtt;

SceneTab::SceneTab(EditorCommonData& commonData) :
  _ui(new Ui::SceneTab),
  _commonData(commonData),
  _dataTreeView(nullptr),
  _environmentTreeView(nullptr),
  _widgetsPlace(nullptr)
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

  _widgetsPlace = _ui->propertiesPlaceLayout;

  connect(&_commonData,
          &EditorCommonData::sceneChanged,
          this,
          &SceneTab::_setScene,
          Qt::DirectConnection);
  _setScene(_commonData.scene());

  connect(&_commonData,
          &EditorCommonData::selectedObjectsChanged,
          this,
          &SceneTab::_updatePropertiesWidgets,
          Qt::DirectConnection);
  _updatePropertiesWidgets();
}

SceneTab::~SceneTab() noexcept
{
  delete _ui;
}

void SceneTab::_setRootObjects(Object* dataRoot, Object* environmentRoot)
{
  _dataTreeView->setItemModel(nullptr);
  _dataItemModel.reset();
  _environmentTreeView->setItemModel(nullptr);
  _environmentItemModel.reset();


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

void SceneTab::_setScene(EditorScene* scene)
{
  if (scene == nullptr) _setRootObjects(nullptr, nullptr);
  else _setRootObjects(&scene->dataRoot(), &scene->environmentRoot());
}

std::unique_ptr<PropertiesWidgetFactory> SceneTab::createWidgetsFactory(
                                                    QVBoxLayout& widgetsLayout)
{
  return std::make_unique<PropertiesWidgetFactory>( widgetsLayout,
                                                    _commonData,
                                                    nullptr,
                                                    nullptr);
}

void SceneTab::_updatePropertiesWidgets() noexcept
{
  _clearPropertiesWidgets();

  if(_commonData.selectedObjects().size() != 1) return;
  if(_commonData.scene() == nullptr) Abort("SceneTab::_updatePropertiesWidgets: scene is null");

  try
  {
    std::unique_ptr<PropertiesWidgetFactory> widgetsFactory =
                                          createWidgetsFactory(*_widgetsPlace);
    if(widgetsFactory == nullptr) Abort("SceneTab::_updatePropertiesWidgets: widgets factory is null.");
    widgetsFactory->process(*_commonData.selectedObjects()[0]);
  }
  catch (std::exception& error)
  {
    Log() << "SceneTab::_updatePropertiesWidgets: " << error.what();
    _clearPropertiesWidgets();
  }
  catch (...)
  {
    Log() << "SceneTab::_updatePropertiesWidgets: unknown error";
    _clearPropertiesWidgets();
  }
}

void SceneTab::_clearPropertiesWidgets() noexcept
{
  try
  {
    QList<QWidget*> childWidgets =
      _ui->propertiesPlace->findChildren<QWidget*>( QString(),
                                                    Qt::FindDirectChildrenOnly);
    for (QWidget* child : childWidgets) delete child;
  }
  catch (std::exception& error)
  {
    Log() << error.what();
    Abort("SceneTab::_clearPropertiesWidgets: unable to clear widgets layout");
  }
  catch (...)
  {
    Abort("SceneTab::_clearPropertiesWidgets: unknown error");
  }
}
