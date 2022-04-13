#pragma once

#include <mtt/editorLib/MainWindow/EditMenu.h>

class ObjectEditorCommonData;

class Ui_MainWindow;

class EditMenu : public mtt::EditMenu
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
  ObjectEditorCommonData& _commonData;
};