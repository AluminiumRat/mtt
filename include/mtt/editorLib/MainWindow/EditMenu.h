#pragma once

#include <memory>

#include <QtWidgets/QMenu>

#include <mtt/application/Clipboard/CopyToClipboardOperation.h>
#include <mtt/application/Clipboard/PasteFromClipboardOperation.h>

namespace mtt
{
  class EditorCommonData;

  class EditMenu : public QMenu
  {
    Q_OBJECT

  public:
    EditMenu(
        QWidget& window,
        EditorCommonData& commonData,
        std::unique_ptr<CopyToClipboardOperation> copyOperation = nullptr,
        std::unique_ptr<PasteFromClipboardOperation> pasteOperation = nullptr);
    EditMenu(const EditMenu&) = delete;
    EditMenu& operator = (const EditMenu&) = delete;
    virtual ~EditMenu() noexcept = default;

    inline QWidget& window() const noexcept;
    inline EditorCommonData& commonData() const noexcept;

  private:
    void _undo() noexcept;
    void _redo() noexcept;
    void _updateCopyDeleteActions() noexcept;
    void _updatePasteAction() noexcept;
    void _copyObject() noexcept;
    void _pasteObject() noexcept;
    void _deleteObject() noexcept;

  private:
    QWidget& _window;
    EditorCommonData& _commonData;
    std::unique_ptr<CopyToClipboardOperation> _copyOperation;
    std::unique_ptr<PasteFromClipboardOperation> _pasteOperation;

    QAction* _copyAction;
    QAction* _pasteAction;
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