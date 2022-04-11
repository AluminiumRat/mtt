#pragma once

#include <memory>
#include <optional>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/BoolCheckboxConnection.h>

#include <Objects/MaterialObject.h>

namespace mtt
{
  class UndoStack;
}

namespace Ui
{
  class MaterialWidget;
}

class MaterialWidget : public QWidget
{
public:
  MaterialWidget(MaterialObject& object, mtt::UndoStack& undoStack);
  MaterialWidget(const MaterialWidget&) = delete;
  MaterialWidget& operator = (const MaterialWidget&) = delete;
  virtual ~MaterialWidget() noexcept;

private:
  std::unique_ptr<Ui::MaterialWidget> _ui;

  using BoolConnection = mtt::BoolCheckboxConnection<MaterialObject>;
  std::optional<BoolConnection> _useAplphaConnection;
};
