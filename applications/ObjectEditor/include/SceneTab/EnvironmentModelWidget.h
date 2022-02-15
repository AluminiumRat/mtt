#pragma once

#include <memory>

#include <QtWidgets/QWidget>

namespace mtt
{
  class EnvironmentModel;
  class UndoStack;
}

namespace Ui
{
  class EnvironmentModelWidget;
}

class EnvironmentModelWidget : public QWidget
{
  Q_OBJECT

public:
  EnvironmentModelWidget( mtt::EnvironmentModel& object,
                          mtt::UndoStack& undoStack);
  EnvironmentModelWidget(const EnvironmentModelWidget&) = delete;
  EnvironmentModelWidget& operator = (const EnvironmentModelWidget&) = delete;
  virtual ~EnvironmentModelWidget() noexcept;

private:
  std::unique_ptr<Ui::EnvironmentModelWidget> _ui;

  mtt::EnvironmentModel& _object;
  mtt::UndoStack& _undoStack;
};
