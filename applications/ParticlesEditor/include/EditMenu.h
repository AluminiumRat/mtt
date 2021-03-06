#pragma once

#include <memory>

#include <mtt/editorLib/MainWindow/EditMenu.h>

#include <Objects/HierarhicalObject.h>

class ParticlesEditorCommonData;
class MainWindow;

class Ui_MainWindow;

class EditMenu : public mtt::EditMenu
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
  ParticlesEditorCommonData& _commonData;
};