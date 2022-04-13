#pragma once

#include <QtWidgets/QMenu>

class ObjectEditorCommonData;

class FileMenu : public QMenu
{
  Q_OBJECT

public:
  FileMenu(QWidget& window, ObjectEditorCommonData& commonData);
  FileMenu(const FileMenu&) = delete;
  FileMenu& operator = (const FileMenu&) = delete;
  virtual ~FileMenu() noexcept = default;

private:
  void _loadModel() noexcept;

  void _saveModel() noexcept;
  void _saveModelAs() noexcept;
  void _saveModelToFile(const QString& file) noexcept;

private:
  QWidget& _window;
  ObjectEditorCommonData& _commonData;
};