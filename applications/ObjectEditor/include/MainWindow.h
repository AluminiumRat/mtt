#pragma once

#include <optional>

#include <QtWidgets/QMainWindow>

#include <mtt/application/Widgets/AsyncTaskDialog/AsyncTaskDialog.h>
#include <mtt/editorLib/Render/EditorRenderWidget.h>
#include <mtt/editorLib/Render/SceneRenderObserver.h>
#include <mtt/editorLib/SceneTreeWidget/SceneTreeWidget.h>
#include <mtt/editorLib/EnvironmentMenu.h>
#include <mtt/editorLib/ManipulatorMenu.h>
#include <mtt/editorLib/RenderMenu.h>

#include <PropertiesWidget/PropertiesWidget.h>
#include <Render/RenderObserverFactory.h>
#include <EditMenu.h>
#include <FileMenu.h>
#include <ObjectEditorCommonData.h>

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow();
  MainWindow(const MainWindow&) = delete;
  MainWindow& operator = (const MainWindow&) = delete;
  virtual ~MainWindow() noexcept = default;

protected:
  virtual void closeEvent(QCloseEvent* event) override;

private:
  void _restoreGeometry();
  void _updateSceneRenderObserver() noexcept;
  void _showAsyncTaskDialog(mtt::AbstractAsyncTask& task);
  void _checkAsyncTaskDialog();
  void _processWarning( mtt::AbstractAsyncTask& task,
                        const QString& message) noexcept;
  void _processError( mtt::AbstractAsyncTask& task,
                      const std::exception& error) noexcept;

private:
  ObjectEditorCommonData _commonEditData;

  RenderObserverFactory _observerFactory;
  std::optional<mtt::SceneRenderObserver> _sceneRenderObserver;

  PropertiesWidget _propertiesWidget;
  mtt::SceneTreeWidget _treeWidget;
  mtt::EditorRenderWidget _renderWidget;

  FileMenu _fileMenu;
  EditMenu _editMenu;
  mtt::EnvironmentMenu _environmentMenu;
  mtt::ManipulatorMenu _manipulatorMenu;
  mtt::RenderMenu _renderMenu;

  mtt::AsyncTaskDialog _asyncTaskDialog;
};