#include <stdexcept>

#include <QtWidgets/QMessageBox>

#include <mtt/application/Widgets/Actions/CopyAction.h>
#include <mtt/application/CommonEditData.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

CopyAction::CopyAction( std::unique_ptr<CopyToClipboardOperation> copyOperation,
                        CommonEditData& commonData,
                        QWidget* dialogParentWidget) :
  QAction(tr("&Copy")),
  _copyOperation(std::move(copyOperation)),
  _commonData(commonData),
  _dialogParentWidget(dialogParentWidget)
{
  if(_copyOperation == nullptr) Abort("CopyAction::CopyAction: copyOperation is nullptr.");

  setShortcut(Qt::CTRL + Qt::Key_C);

  connect(&_commonData,
          &CommonEditData::selectedObjectsChanged,
          this,
          &CopyAction::_updatEnabled,
          Qt::DirectConnection);
  _updatEnabled();

  connect(this,
          &CopyAction::triggered,
          this,
          &CopyAction::_makeCopy,
          Qt::DirectConnection);
}

void CopyAction::_updatEnabled() noexcept
{
  bool enabledValue = false;

  for(Object* object : _commonData.selectedObjects())
  {
    if(_copyOperation->objectSupported(*object))
    {
      enabledValue = true;
    }
  }

  setEnabled(enabledValue);
}

void CopyAction::_makeCopy() noexcept
{
  try
  {
    _copyOperation->copyToClipboard(_commonData.selectedObjects());
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
