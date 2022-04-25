#include <memory>

#include <QtCore/QSettings>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QMenuBar>

#include <mtt/editorLib/MainWindow/ManipulatorToolbar.h>
#include <mtt/editorLib/Objects/DirectLightObject.h>
#include <mtt/utilities/Log.h>

#include <MainWindow.h>
#include <PlayerToolbar.h>

MainWindow::MainWindow() :
  _observerFactory(_commonEditData),
  _treeWidget(_commonEditData),
  _propertiesWidget(_commonEditData),
  _renderWidget(_commonEditData.renderScene(), _commonEditData),
  _fileMenu(*this, _commonEditData),
  _editMenu(*this, _commonEditData),
  _environmentMenu(*this, _commonEditData),
  _manipulatorMenu(*this, _renderWidget.manipulatorController()),
  _renderMenu(_renderWidget)
{
  setWindowTitle(tr("Particles editor"));

  menuBar()->addMenu(&_fileMenu);
  menuBar()->addMenu(&_editMenu);
  menuBar()->addMenu(&_environmentMenu);
  menuBar()->addMenu(&_manipulatorMenu);
  menuBar()->addMenu(&_renderMenu);

  std::unique_ptr<PlayerToolbar> playerToolbar(
                                            new PlayerToolbar(_commonEditData));
  playerToolbar->setObjectName("PlayerToolbar");
  playerToolbar->setMovable(false);
  addToolBar(playerToolbar.release());

  std::unique_ptr<mtt::ManipulatorToolbar> manipulatorToolbar(
           new mtt::ManipulatorToolbar( *this,
                                        _renderWidget.manipulatorController()));
  manipulatorToolbar->setObjectName("ManipulatorToolbar");
  manipulatorToolbar->setMovable(false);
  addToolBar(manipulatorToolbar.release());

  connect(&_commonEditData,
          &ParticlesEditorCommonData::sceneChanged,
          this,
          &MainWindow::_updateSceneRenderObserver,
          Qt::DirectConnection);
  _updateSceneRenderObserver();

  std::unique_ptr<QDockWidget> treeDock(new QDockWidget(this));
  treeDock->setObjectName("TreeDock");
  treeDock->setFeatures(
            QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
  treeDock->setWindowTitle(tr("Scene"));
  treeDock->setWidget(&_treeWidget);
  treeDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  addDockWidget(Qt::RightDockWidgetArea, treeDock.release());

  std::unique_ptr<QDockWidget> propertiesDock(new QDockWidget(this));
  propertiesDock->setObjectName("PropertiesDock");
  propertiesDock->setFeatures(
            QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
  propertiesDock->setWindowTitle(tr("Properties"));
  propertiesDock->setWidget(&_propertiesWidget);
  propertiesDock->setAllowedAreas(
                              Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  addDockWidget(Qt::RightDockWidgetArea, propertiesDock.release());

  setCentralWidget(&_renderWidget);

  std::unique_ptr<mtt::DirectLightObject> directLight(
                          new mtt::DirectLightObject(tr("Direct light"), true));
  directLight->setPosition(glm::vec3(0, -2, 4));
  directLight->setRotation(glm::quat(glm::vec3(.5f, 0.f, 0.f)));

  std::unique_ptr<ParticlesEditorScene> scene(new ParticlesEditorScene);
  scene->environmentRoot().objects().addChild(std::move(directLight));
  _commonEditData.setScene(std::move(scene));

  _restoreGeometry();
}

void MainWindow::_restoreGeometry()
{
  QSettings settings("MTT", "ParticlesEditor");
  restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
  restoreState(settings.value("mainWindowState").toByteArray());
}

void MainWindow::closeEvent(QCloseEvent* event)
{
  QSettings settings("MTT", "ParticlesEditor");
  settings.setValue("mainWindowGeometry", saveGeometry());
  settings.setValue("mainWindowState", saveState());
  QMainWindow::closeEvent(event);
}

void MainWindow::_updateSceneRenderObserver() noexcept
{
  _sceneRenderObserver.reset();
  if(_commonEditData.scene() == nullptr) return;

  try
  {
    _sceneRenderObserver.emplace( *_commonEditData.scene(),
                                  _commonEditData.renderScene(),
                                  _observerFactory);
  }
  catch (std::exception& error)
  {
    mtt::Log() << "MainWindow::_updateSceneRenderObserver: unable to update scene observer: " << error.what();
  }
  catch (...)
  {
    mtt::Log() << "MainWindow::_updateSceneRenderObserver: unable to update scene observer: unknown error";
  }
}
