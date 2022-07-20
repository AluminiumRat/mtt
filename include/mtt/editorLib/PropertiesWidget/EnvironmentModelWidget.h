#pragma once

#include <memory>

#include <QtWidgets/QWidget>

namespace Ui
{
  class EnvironmentModelWidget;
}

namespace mtt
{
  class EnvironmentModel;
  class UndoStack;

  class EnvironmentModelWidget : public QWidget
  {
    Q_OBJECT

  public:
    EnvironmentModelWidget( EnvironmentModel& object,
                            UndoStack& undoStack);
    EnvironmentModelWidget(const EnvironmentModelWidget&) = delete;
    EnvironmentModelWidget& operator = (const EnvironmentModelWidget&) = delete;
    virtual ~EnvironmentModelWidget() noexcept;

  private:
    void _reloadModel() noexcept;

  private:
    std::unique_ptr<Ui::EnvironmentModelWidget> _ui;

    EnvironmentModel& _object;
    UndoStack& _undoStack;
  };
}