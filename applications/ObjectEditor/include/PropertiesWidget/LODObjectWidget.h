#pragma once

#include <memory>
#include <optional>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/FloatSpinConnection.h>

#include <Objects/LODObject.h>

namespace mtt
{
  class UndoStack;
}

namespace Ui
{
  class LODObjectWidget;
}

class LODObjectWidget : public QWidget
{
public:
  LODObjectWidget(LODObject& object, mtt::UndoStack& undoStack);
  LODObjectWidget(const LODObjectWidget&) = delete;
  LODObjectWidget& operator = (const LODObjectWidget&) = delete;
  virtual ~LODObjectWidget() noexcept;

private:
  std::unique_ptr<Ui::LODObjectWidget> _ui;

  using FloatConnection = mtt::FloatSpinConnection<LODObject>;
  std::optional<FloatConnection> _minConnection;
  std::optional<FloatConnection> _maxConnection;
};
