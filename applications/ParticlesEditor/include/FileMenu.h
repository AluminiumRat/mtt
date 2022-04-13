#pragma once

#include <QtWidgets/QMenu>

class ParticlesEditorCommonData;

class FileMenu : public QMenu
{
  Q_OBJECT

public:
  FileMenu(QWidget& window, ParticlesEditorCommonData& commonData);
  FileMenu(const FileMenu&) = delete;
  FileMenu& operator = (const FileMenu&) = delete;
  virtual ~FileMenu() noexcept = default;

private:
  void _load() noexcept;

  void _saveEffect() noexcept;
  void _saveEffectAs() noexcept;
  void _saveToFile(const QString& file) noexcept;

private:
  QWidget& _window;
  ParticlesEditorCommonData& _commonData;
};