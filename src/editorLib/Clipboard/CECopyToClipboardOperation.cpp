#include <mtt/editorLib/Clipboard/CECopyToClipboardOperation.h>
#include <mtt/editorLib/Objects/CEObjectSaver.h>
#include <mtt/editorLib/Objects/EnvironmentObject.h>
#include <mtt/editorLib/Objects/EnvironmentObjectFactory.h>

using namespace mtt;

CECopyToClipboardOperation::CECopyToClipboardOperation()
{
  addSaver( environmentCategoryName,
            std::make_unique<CEObjectSaver>(),
            std::make_unique<EnvironmentObjectFactory>());
}

bool CECopyToClipboardOperation::objectSupported(
                                            const Object& object) const noexcept
{
  return qobject_cast<const EnvironmentObject*>(&object) != nullptr;
}
