#include <stdexcept>

#include <QtGui/QClipboard>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>

#include <mtt/application/Widgets/Actions/PasteAction.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

PasteAction::PasteAction(
                    std::unique_ptr<PasteFromClipboardOperation> pasteOperation,
                    QWidget* dialogParentWidget) :
  QAction(tr("&Paste")),
  _pasteOperation(std::move(pasteOperation)),
  _dialogParentWidget(dialogParentWidget)
{
  if(_pasteOperation == nullptr) Abort("PasteAction::PasteAction: pasteOperation is nullptr.");

  setShortcut(Qt::CTRL + Qt::Key_V);

  QClipboard* clipboard = QApplication::clipboard();
  if (clipboard != nullptr)
  {
    connect(clipboard,
            &QClipboard::dataChanged,
            this,
            &PasteAction::_updatEnabled,
            Qt::DirectConnection);
  }
  _updatEnabled();

  connect(this,
          &PasteAction::triggered,
          this,
          &PasteAction::_makePaste,
          Qt::DirectConnection);
}

void PasteAction::_updatEnabled() noexcept
{
  setEnabled(_pasteOperation->isPasteAvailable());
}

void PasteAction::_makePaste() noexcept
{
  try
  {
    _pasteOperation->pasteFromClipboard();
  }
  catch (std::exception& error)
  {
    QMessageBox::critical(_dialogParentWidget, tr("Error"), error.what());
  }
  catch (...)
  {
    QMessageBox::critical(_dialogParentWidget,
                          tr("Error"),
                          tr("Unknown error"));
  }
}
