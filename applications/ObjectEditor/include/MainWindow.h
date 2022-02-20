#pragma once

#include <optional>

#include <QtWidgets/QMainWindow>

#include <mtt/application/Widgets/AsyncTaskDialog/AsyncTaskDialog.h>
#include <mtt/application/Widgets/StatisticWidget.h>
#include <mtt/editorLib/Render/SceneRenderObserver.h>

#include <RenderWidget/EditorRenderWidget.h>
#include <Render/RenderObserverFactory.h>
#include <SceneTab/SceneTab.h>
#include <EditMenu.h>
#include <EditorCommonData.h>
#include <FileMenu.h>
#include <ManipulatorMenu.h>

class Ui_MainWindow;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow();
  MainWindow(const MainWindow&) = delete;
  MainWindow& operator = (const MainWindow&) = delete;
  virtual ~MainWindow() noexcept;

private:
  void _updateSceneRenderObserver() noexcept;
  void _showAsyncTaskDialog(mtt::AbstractAsyncTask& task);
  void _checkAsyncTaskDialog();
  void _processWarning( mtt::AbstractAsyncTask& task,
                        const QString& message) noexcept;
  void _processError( mtt::AbstractAsyncTask& task,
                      const std::exception& error) noexcept;
  void _showStatistic() noexcept;

private:
  Ui_MainWindow* _ui;

  EditorCommonData _commonEditData;

  RenderObserverFactory _observerFactory;
  std::optional<mtt::SceneRenderObserver> _sceneRenderObserver;

  SceneTab _sceneTab;
  EditorRenderWidget _renderWidget;

  FileMenu _fileMenu;
  EditMenu _editMenu;
  ManipulatorMenu _manipulatorMenu;

  mtt::AsyncTaskDialog _asyncTaskDialog;

  mtt::StatisticWidget _statisticWidget;
};