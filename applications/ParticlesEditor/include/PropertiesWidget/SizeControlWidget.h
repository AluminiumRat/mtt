#pragma once

#include <memory>

#include <QtWidgets/QWidget>

#include <mtt/application/Widgets/PropertiesWidgets/FloatSpinConnection.h>

#include <Objects/SizeControlObject.h>

namespace mtt
{
  class UndoStack;
}

namespace Ui
{
  class SizeControlWidget;
}

class SizeControlWidget : public QWidget
{
public:
  SizeControlWidget(SizeControlObject& object, mtt::UndoStack& undoStack);
  SizeControlWidget(const SizeControlWidget&) = delete;
  SizeControlWidget& operator = (const SizeControlWidget&) = delete;
  virtual ~SizeControlWidget() noexcept;

private:
  std::unique_ptr<Ui::SizeControlWidget> _ui;

  using FloatConnection = mtt::FloatSpinConnection<SizeControlObject>;
  std::optional<FloatConnection> _startSizeConnection;
  std::optional<FloatConnection> _endSizeConnection;
};