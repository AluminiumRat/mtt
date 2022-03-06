#pragma once

#include <QtCore/QObject>

class EditorCommonData;
class MainWindow;

class Ui_MainWindow;

class EditMenu : public QObject
{
  Q_OBJECT

public:
  EditMenu( MainWindow& window,
            Ui_MainWindow& ui,
            EditorCommonData& commonData);
  EditMenu(const EditMenu&) = delete;
  EditMenu& operator = (const EditMenu&) = delete;
  virtual ~EditMenu() noexcept = default;

  void setupUI();

private:
  void _undo() noexcept;
  void _redo() noexcept;
  void _updateDeleteAction() noexcept;
  void _deleteObject() noexcept;
  void _addBone() noexcept;
  void _addAnimationFromFbx() noexcept;

private:
  MainWindow& _window;
  Ui_MainWindow& _ui;
  EditorCommonData& _commonData;
};