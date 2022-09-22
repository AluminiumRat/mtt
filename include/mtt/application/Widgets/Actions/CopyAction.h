#pragma once

#include <memory>

#include <QtWidgets/QAction>

#include <mtt/application/Clipboard/CopyToClipboardOperation.h>

namespace mtt
{
  class CommonEditData;

  class CopyAction : public QAction
  {
    Q_OBJECT

  public:
    /// dialogParentWidget is just parent widget for error dialog. This is not
    /// parent object for QAction.
    /// copyOperation must not be nullptr
    CopyAction( std::unique_ptr<CopyToClipboardOperation> copyOperation,
                CommonEditData& commonData,
                QWidget* dialogParentWidget);
    CopyAction(const CopyAction&) = delete;
    CopyAction& operator = (const CopyAction&) = delete;
    virtual ~CopyAction() noexcept = default;

  private:
    void _updatEnabled() noexcept;
    void _makeCopy() noexcept;

  private:
    std::unique_ptr<CopyToClipboardOperation> _copyOperation;
    CommonEditData& _commonData;
    QWidget* _dialogParentWidget;
  };
}