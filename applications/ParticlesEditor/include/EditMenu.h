#pragma once

#include <memory>

#include <QtCore/QObject>

#include <Objects/HierarhicalObject.h>

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
  void _addHierarhical(std::unique_ptr<HierarhicalObject> object);
  void _addFrame() noexcept;
  template <typename Modificator>
  void _addModificator( const QString& name,
                        const QString& errorString) noexcept;
  void _addEmitter() noexcept;
  void _addVisibilityControl() noexcept;
  void _addGravity() noexcept;
  void _addBlocker() noexcept;
  void _addHeater() noexcept;
  void _addGasSource() noexcept;
  void _addAnimationFromFbx() noexcept;

private:
  MainWindow& _window;
  Ui_MainWindow& _ui;
  EditorCommonData& _commonData;
};