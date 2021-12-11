#pragma once

#include <QtCore/QObject>

class EditorCommonData;
class MainWindow;

class Ui_MainWindow;

class FileMenu : public QObject
{
  Q_OBJECT

public:
  FileMenu( MainWindow& window,
            Ui_MainWindow& ui,
            EditorCommonData& commonData);
  FileMenu(const FileMenu&) = delete;
  FileMenu& operator = (const FileMenu&) = delete;
  virtual ~FileMenu() noexcept = default;

  void setupUI();

private:
  void _saveModel() noexcept;
  void _saveModelAs() noexcept;
  void _save(const QString& file) noexcept;
  void _loadModel() noexcept;

private:
  MainWindow& _window;
  Ui_MainWindow& _ui;
  EditorCommonData& _commonData;
};