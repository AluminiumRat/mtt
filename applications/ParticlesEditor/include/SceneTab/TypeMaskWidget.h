#pragma once

#include <memory>
#include <optional>
#include <variant>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/Bitfield32PropertyConnection.h>

#include <Objects/Fluid/FluidObject.h>
#include <Objects/EmitterObject.h>
#include <Objects/ModificatorObject.h>

namespace mtt
{
  class UndoStack;
}

namespace Ui
{
  class TypeMaskWidget;
}

class TypeMaskWidget : public QWidget
{
public:
  TypeMaskWidget(ModificatorObject& object, mtt::UndoStack& undoStack);
  TypeMaskWidget(EmitterObject& object, mtt::UndoStack& undoStack);
  TypeMaskWidget(FluidObject& object, mtt::UndoStack& undoStack);
  TypeMaskWidget(const TypeMaskWidget&) = delete;
  TypeMaskWidget& operator = (const TypeMaskWidget&) = delete;
  virtual ~TypeMaskWidget() noexcept;

private:
  std::unique_ptr<Ui::TypeMaskWidget> _ui;

  using ModificatorConnection =
                          mtt::Bitfield32PropertyConnection<ModificatorObject>;

  using EmitterConnection = mtt::Bitfield32PropertyConnection<EmitterObject>;
  using FluidConnection = mtt::Bitfield32PropertyConnection<FluidObject>;

  std::variant< int,
                ModificatorConnection,
                EmitterConnection,
                FluidConnection> _typemaskConnection;
};
