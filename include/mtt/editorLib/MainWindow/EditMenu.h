#pragma once

#include <QtWidgets/QMenu>

namespace mtt
{
  class EditorCommonData;

  class EditMenu : public QMenu
  {
    Q_OBJECT

  public:
    EditMenu(QWidget& window, EditorCommonData& commonData);
    EditMenu(const EditMenu&) = delete;
    EditMenu& operator = (const EditMenu&) = delete;
    virtual ~EditMenu() noexcept = default;

    inline QWidget& window() const noexcept;
    inline EditorCommonData& commonData() const noexcept;

  private:
    void _undo() noexcept;
    void _redo() noexcept;
    void _updateDeleteAction() noexcept;
    void _deleteObject() noexcept;

  private:
    QWidget& _window;
    EditorCommonData& _commonData;

    QAction* _deleteAction;
  };

  inline QWidget& EditMenu::window() const noexcept
  {
    return _window;
  }

  inline EditorCommonData& EditMenu::commonData() const noexcept
  {
    return _commonData;
  }
}