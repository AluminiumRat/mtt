#pragma once

#include <QtCore/QObject>

class ParticlesEditorCommonData;
class MainWindow;

class Ui_MainWindow;

class FileMenu : public QObject
{
  Q_OBJECT

public:
  FileMenu( MainWindow& window,
            Ui_MainWindow& ui,
            ParticlesEditorCommonData& commonData);
  FileMenu(const FileMenu&) = delete;
  FileMenu& operator = (const FileMenu&) = delete;
  virtual ~FileMenu() noexcept = default;

  void setupUI();

private:
  void _saveEffect() noexcept;
  void _saveEffectAs() noexcept;
  void _saveToFile(const QString& file) noexcept;
  void _load() noexcept;

  void _loadEnvironment() noexcept;

private:
  MainWindow& _window;
  Ui_MainWindow& _ui;
  ParticlesEditorCommonData& _commonData;
};