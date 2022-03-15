#pragma once

#include <memory>
#include <optional>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/Bitfield32PropertyConnection.h>

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
  TypeMaskWidget(const TypeMaskWidget&) = delete;
  TypeMaskWidget& operator = (const TypeMaskWidget&) = delete;
  virtual ~TypeMaskWidget() noexcept;

private:
  std::unique_ptr<Ui::TypeMaskWidget> _ui;

  using TypeMaskConnection = mtt::Bitfield32PropertyConnection<ModificatorObject>;
  std::optional<TypeMaskConnection> _typemaskConnection;
};
