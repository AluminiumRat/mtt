#pragma once

#include <optional>

#include <QtWidgets/QMainWindow>

#include <mtt/editorLib/MainWindow/EditorMainWindow.h>
#include <mtt/editorLib/MainWindow/EnvironmentMenu.h>
#include <mtt/editorLib/MainWindow/RenderMenu.h>
#include <mtt/editorLib/Render/EditorRenderWidget.h>
#include <mtt/editorLib/Render/SceneRenderObserver.h>
#include <mtt/editorLib/SceneTreeWidget/SceneTreeWidget.h>

#include <PropertiesWidget/PropertiesWidget.h>
#include <Render/RenderObserverFactory.h>
#include <EditMenu.h>
#include <FileMenu.h>
#include <ObjectEditorCommonData.h>

class MainWindow : public mtt::EditorMainWindow
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
  mtt::RenderMenu _renderMenu;
};