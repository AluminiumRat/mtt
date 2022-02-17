#include <stdexcept>

#include <QtWidgets/QVBoxLayout>

#include <mtt/application/Widgets/ObjectsTreeView.h>
#include <mtt/application/CommonEditData.h>
#include <mtt/editorLib/SceneTab/SceneTab.h>
#include <mtt/utilities/Abort.h>

#include <GeneratedFiles/ui_SceneTab.h>

using namespace mtt;

SceneTab::SceneTab(CommonEditData& commonEditData) :
  _ui(new Ui::sceneTab),
  _commonEditData(commonEditData),
  _treeView(nullptr),
  _widgetsPlace(nullptr)
{
  _ui->setupUi(this);

  std::unique_ptr<SceneTreeView> treeView(new SceneTreeView(&_commonEditData));
  _treeView = treeView.get();
  _ui->objectTreePlace->layout()->addWidget(_treeView);
  treeView.release();
  _treeView->setSortFilterModel(&_sortModel);
  _treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
  _treeView->setDragEnabled(true);
  _treeView->setDragDropOverwriteMode(false);
  _treeView->setDragDropMode(QAbstractItemView::InternalMove);
  _treeView->setDefaultDropAction(Qt::MoveAction);

  _widgetsPlace = _ui->propertiesPlaceLayout;

  connect(&_commonEditData,
          &CommonEditData::selectedObjectsChanged,
          this,
          &SceneTab::_updatePropertiesWidgets,
          Qt::DirectConnection);
  _updatePropertiesWidgets();
}

SceneTab::~SceneTab() noexcept
{
  delete _ui;
}

void SceneTab::setRootObject(Object* newRoot)
{
  _treeView->setItemModel(nullptr);
  _itemModel.reset();

  if (newRoot != nullptr)
  {
    _itemModel.emplace(*newRoot, &_commonEditData.undoStack());
    _treeView->setItemModel(&_itemModel.value());
  }
}

std::unique_ptr<PropertiesWidgetFactory> SceneTab::createWidgetsFactory(
                                                    QVBoxLayout& widgetsLayout)
{
  return std::make_unique<PropertiesWidgetFactory>( widgetsLayout,
                                                    _commonEditData,
                                                    nullptr,
                                                    nullptr);
}

void SceneTab::_updatePropertiesWidgets() noexcept
{
  _clearPropertiesWidgets();

  if(_commonEditData.selectedObjects().size() != 1) return;
  if(_commonEditData.scene() == nullptr) Abort("SceneTab::_updatePropertiesWidgets: scene is null");

  try
  {
    std::unique_ptr<PropertiesWidgetFactory> widgetsFactory =
                                          createWidgetsFactory(*_widgetsPlace);
    if(widgetsFactory == nullptr) Abort("SceneTab::_updatePropertiesWidgets: widgets factory is null.");
    widgetsFactory->process(*_commonEditData.selectedObjects()[0]);
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
