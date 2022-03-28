#pragma once

#include <memory>
#include <optional>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/EnumComboConnection.h>
#include <mtt/application/Widgets/PropertiesWidgets/FloatSpinConnection.h>

#include <Objects/BlockerObject.h>

namespace mtt
{
  class UndoStack;
}

namespace Ui
{
  class BlockerWidget;
}

class BlockerWidget : public QWidget
{
public:
  BlockerWidget(BlockerObject& object, mtt::UndoStack& undoStack);
  BlockerWidget(const BlockerWidget&) = delete;
  BlockerWidget& operator = (const BlockerWidget&) = delete;
  virtual ~BlockerWidget() noexcept;

private:
  std::unique_ptr<Ui::BlockerWidget> _ui;

  using ShapeConnection = mtt::EnumComboConnection< BlockerObject,
                                                    BlockerObject::Shape>;
  std::optional<ShapeConnection> _shapeConnection;

  using FloatConnection = mtt::FloatSpinConnection<BlockerObject>;
  std::optional<FloatConnection> _sizeConnection;
};