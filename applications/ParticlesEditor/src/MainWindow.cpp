#include <memory>

#include <QtCore/QSettings>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QMessageBox>

#include <mtt/editorLib/Objects/DirectLightObject.h>
#include <mtt/editorLib/EditorApplication.h>
#include <mtt/utilities/Log.h>

#include <MainWindow.h>

#include <GeneratedFiles/ui_MainWindow.h>

MainWindow::MainWindow() :
  _ui(new Ui_MainWindow),
  _observerFactory(_commonEditData),
  _treeWidget(_commonEditData),
  _propertiesWidget(_commonEditData),
  _renderWidget(_commonEditData.renderScene(), _commonEditData),
  _fileMenu(*this, *_ui, _commonEditData),
  _editMenu(*this, *_ui, _commonEditData),
  _manipulatorMenu(*this, _renderWidget.manipulatorController(), *_ui),
  _asyncTaskDialog(mtt::EditorApplication::instance().asyncTaskQueue)
{
  _ui->setupUi(this);

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

  _fileMenu.setupUI();
  _editMenu.setupUI();
  _manipulatorMenu.setupUI();

  _asyncTaskDialog.setModal(true);

  connect(&mtt::EditorApplication::instance().asyncTaskQueue,
          &mtt::AsyncTaskQueue::taskStarted,
          this,
          &MainWindow::_showAsyncTaskDialog,
          Qt::DirectConnection);

  connect(&mtt::EditorApplication::instance().asyncTaskQueue,
          &mtt::AsyncTaskQueue::taskFinished,
          this,
          &MainWindow::_checkAsyncTaskDialog,
          Qt::DirectConnection);

  connect(&mtt::EditorApplication::instance().asyncTaskQueue,
          &mtt::AsyncTaskQueue::warningEmitted,
          this,
          &MainWindow::_processWarning);

  connect(&mtt::EditorApplication::instance().asyncTaskQueue,
          &mtt::AsyncTaskQueue::errorEmitted,
          this,
          &MainWindow::_processError);

  connect(_ui->actionStatistic,
          &QAction::triggered,
          this,
          &MainWindow::_showStatistic,
          Qt::DirectConnection);

  _restoreGeometry();
}

MainWindow::~MainWindow() noexcept
{
  delete _ui;
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

void MainWindow::_showAsyncTaskDialog(mtt::AbstractAsyncTask& task)
{
  if(task.displayBehavior() == mtt::AbstractAsyncTask::EXPLICIT)
  {
    _asyncTaskDialog.show();
  }
}

void MainWindow::_checkAsyncTaskDialog()
{
  mtt::AsyncTaskQueue::Counters counters =
                  mtt::EditorApplication::instance().asyncTaskQueue.counters();
  if(counters.explicitFinishedInChunk == counters.explicitInChunk)
  {
    _asyncTaskDialog.hide();
  }
}

void MainWindow::_processWarning( mtt::AbstractAsyncTask& task,
                                  const QString& message) noexcept
{
  QMessageBox* dialog = new QMessageBox(&_asyncTaskDialog);
  dialog->setWindowTitle(task.name());
  dialog->setText(message);
  dialog->setIcon(QMessageBox::Warning);
  dialog->setModal(true);
  dialog->show();
}

void MainWindow::_processError( mtt::AbstractAsyncTask& task,
                                const std::exception& error) noexcept
{
  QMessageBox* errorDialog = new QMessageBox(&_asyncTaskDialog);
  errorDialog->setWindowTitle(task.name());
  errorDialog->setText(error.what());
  errorDialog->setIcon(QMessageBox::Critical);
  errorDialog->setModal(true);
  errorDialog->show();
}

void MainWindow::_showStatistic() noexcept
{
  _statisticWidget.show();
  _statisticWidget.setFixedSize(_statisticWidget.size());
}
