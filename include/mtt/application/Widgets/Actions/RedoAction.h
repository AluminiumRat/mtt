#pragma once

#include <QtWidgets/QAction>

namespace mtt
{
  class UndoStack;

  class RedoAction : public QAction
  {
    Q_OBJECT

  public:
    /// dialogParentWidget is just parent widget for error dialog. This is not
    /// parent object for QAction.
    RedoAction(UndoStack& undoStack, QWidget* dialogParentWidget);
    RedoAction(const RedoAction&) = delete;
    RedoAction& operator = (const RedoAction&) = delete;
    virtual ~RedoAction() noexcept = default;

  private:
    void _makeRedo() noexcept;

  private:
    UndoStack& _undoStack;
    QWidget* _dialogParentWidget;
  };
}