#pragma once

#include <QtWidgets/QAction>

namespace mtt
{
  class UndoStack;

  class UndoAction : public QAction
  {
    Q_OBJECT

  public:
    /// dialogParentWidget is just parent widget for error dialog. This is not
    /// parent object for QAction.
    UndoAction(UndoStack& undoStack, QWidget* dialogParentWidget);
    UndoAction(const UndoAction&) = delete;
    UndoAction& operator = (const UndoAction&) = delete;
    virtual ~UndoAction() noexcept = default;

  private:
    void _makeUndo() noexcept;

  private:
    UndoStack& _undoStack;
    QWidget* _dialogParentWidget;
  };
}