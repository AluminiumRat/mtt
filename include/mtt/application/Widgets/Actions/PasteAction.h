#pragma once

#include <memory>

#include <QtWidgets/QAction>

#include <mtt/application/Clipboard/PasteFromClipboardOperation.h>

namespace mtt
{
  class PasteAction : public QAction
  {
    Q_OBJECT

  public:
    /// dialogParentWidget is just parent widget for error dialog. This is not
    /// parent object for QAction.
    /// pasteOperation must not be nullptr
    PasteAction(std::unique_ptr<PasteFromClipboardOperation> pasteOperation,
                QWidget* dialogParentWidget);
    PasteAction(const PasteAction&) = delete;
    PasteAction& operator = (const PasteAction&) = delete;
    virtual ~PasteAction() noexcept = default;

  private:
    void _updatEnabled() noexcept;
    void _makePaste() noexcept;

  private:
    std::unique_ptr<PasteFromClipboardOperation> _pasteOperation;
    QWidget* _dialogParentWidget;
  };
}