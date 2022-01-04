#include <memory>

#include <QtWidgets/QDockWidget>
#include <QtWidgets/QMessageBox>

#include <Objects/DirectLightObject.h>
#include <EditorApplication.h>
#include <MainWindow.h>

#include <GeneratedFiles/ui_MainWindow.h>

MainWindow::MainWindow() :
  _ui(new Ui_MainWindow),
  _sceneRenderObserver(_commonEditData),
  _sceneTab(_commonEditData),
  _renderWidget(_commonEditData),
  _fileMenu(*this, *_ui, _commonEditData),
  _editMenu(*this, *_ui, _commonEditData),
  _manipulatorMenu(*this, _renderWidget.manipulatorController(), *_ui),
  _asyncTaskDialog(EditorApplication::instance().asyncTaskQueue)
{
  _ui->setupUi(this);

  std::unique_ptr<QDockWidget> dockWidget(new QDockWidget(this));
  QDockWidget* dock = dockWidget.get();
  addDockWidget(Qt::RightDockWidgetArea, dock);
  dockWidget.release();
  dock->setFeatures(QDockWidget::NoDockWidgetFeatures);
  dock->setWindowTitle(tr("Properties"));
  dock->show();
  dock->setWidget(&_sceneTab);
  _sceneTab.show();

  setCentralWidget(&_renderWidget);

  std::unique_ptr<DirectLightObject> directLight(
                              new DirectLightObject(tr("Direct light"), true));
  directLight->setPosition(glm::vec3(0, -2, 4));
  directLight->setRotation(glm::quat(glm::vec3(.5f, 0.f, 0.f)));

  std::unique_ptr<EditorScene> scene(new EditorScene);
  scene->root().environment().addChild(std::move(directLight));
  _commonEditData.setScene(std::move(scene));

  _fileMenu.setupUI();
  _editMenu.setupUI();
  _manipulatorMenu.setupUI();

  _asyncTaskDialog.setModal(true);

  connect(&EditorApplication::instance().asyncTaskQueue,
          &mtt::AsyncTaskQueue::taskStarted,
          this,
          &MainWindow::_showAsyncTaskDialog,
          Qt::DirectConnection);

  connect(&EditorApplication::instance().asyncTaskQueue,
          &mtt::AsyncTaskQueue::taskFinished,
          this,
          &MainWindow::_checkAsyncTaskDialog,
          Qt::DirectConnection);

  connect(&EditorApplication::instance().asyncTaskQueue,
          &mtt::AsyncTaskQueue::warningEmitted,
          this,
          &MainWindow::_processWarning);

  connect(&EditorApplication::instance().asyncTaskQueue,
          &mtt::AsyncTaskQueue::errorEmitted,
          this,
          &MainWindow::_processError);

  connect(_ui->actionStatistic,
          &QAction::triggered,
          this,
          &MainWindow::_showStatistic,
          Qt::DirectConnection);
}

MainWindow::~MainWindow() noexcept
{
  delete _ui;
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
                        EditorApplication::instance().asyncTaskQueue.counters();
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
