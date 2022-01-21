#pragma once

#include <memory>

#include <QtWidgets/QWidget>

namespace Ui
{
  class NamePropertyWidget;
}

namespace mtt
{
  class Object;
  class UndoStack;

  class NamePropertyWidget : public QWidget
  {
    Q_OBJECT

  public:
    NamePropertyWidget(Object& object, UndoStack& undoStack);
    NamePropertyWidget(const NamePropertyWidget&) = delete;
    NamePropertyWidget& operator = (const NamePropertyWidget&) = delete;
    virtual ~NamePropertyWidget() noexcept;
  
  private:
    void _updateObject() noexcept;
    void _updateWidget() noexcept;

  private:
    std::unique_ptr<Ui::NamePropertyWidget> _ui;
    Object& _object;
    UndoStack& _undoStack;
    bool _skipUpdate;
  };
}