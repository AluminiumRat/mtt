#pragma once

#include <memory>

#include <QtWidgets/QMenu>

#include <mtt/application/Widgets/Actions/CopyAction.h>
#include <mtt/application/Widgets/Actions/DeleteAction.h>
#include <mtt/application/Widgets/Actions/PasteAction.h>
#include <mtt/application/Widgets/Actions/RedoAction.h>
#include <mtt/application/Widgets/Actions/UndoAction.h>

#include <Objects/HierarhicalObject.h>

class ParticlesEditorCommonData;

class EditMenu : public QMenu
{
  Q_OBJECT

public:
  EditMenu(QWidget& window, ParticlesEditorCommonData& commonData);
  EditMenu(const EditMenu&) = delete;
  EditMenu& operator = (const EditMenu&) = delete;
  virtual ~EditMenu() noexcept = default;

private:
  void _addHierarhical(std::unique_ptr<HierarhicalObject> object);
  void _addFrame() noexcept;
  template <typename Modificator>
  void _addModificator( const QString& name,
                        const QString& errorString) noexcept;
  void _addEmitter() noexcept;
  void _addVisibilityControl() noexcept;
  void _addSizeControl() noexcept;
  void _addGravity() noexcept;
  void _addBlocker() noexcept;
  void _addHeater() noexcept;
  void _addGasSource() noexcept;
  void _addAnimationFromFbx() noexcept;
  template <typename Action>
  void _addAction(const QString& name,
                  const QString& errorString) noexcept;
  void _addParticlesEmissionAction() noexcept;
  void _addGasEmissionAction() noexcept;
  void _addHeatingAction() noexcept;

private:
  QWidget& _window;
  ParticlesEditorCommonData& _commonData;

  mtt::UndoAction _undoAction;
  mtt::RedoAction _redoAction;
  mtt::CopyAction _copyAction;
  mtt::PasteAction _pasteAction;
  mtt::DeleteAction _deleteAction;
};