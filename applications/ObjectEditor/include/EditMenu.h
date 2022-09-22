#pragma once

#include <QtWidgets/QMenu>

#include <mtt/application/Widgets/Actions/CopyAction.h>
#include <mtt/application/Widgets/Actions/DeleteAction.h>
#include <mtt/application/Widgets/Actions/PasteAction.h>
#include <mtt/application/Widgets/Actions/RedoAction.h>
#include <mtt/application/Widgets/Actions/UndoAction.h>

class ObjectEditorCommonData;

class EditMenu : public QMenu
{
  Q_OBJECT

public:
  EditMenu(QWidget& window, ObjectEditorCommonData& commonData);
  EditMenu(const EditMenu&) = delete;
  EditMenu& operator = (const EditMenu&) = delete;
  virtual ~EditMenu() noexcept = default;

private:
  void _addBone() noexcept;
  void _addLOD() noexcept;
  void _addMaterial() noexcept;
  void _addModelFromBlender() noexcept;
  void _addModelFrom3DMax() noexcept;
  void _addModelFromObj() noexcept;
  void _addAnimationFromFbx() noexcept;

private:
  QWidget& _window;
  ObjectEditorCommonData& _commonData;

  mtt::UndoAction _undoAction;
  mtt::RedoAction _redoAction;
  mtt::CopyAction _copyAction;
  mtt::PasteAction _pasteAction;
  mtt::DeleteAction _deleteAction;
};