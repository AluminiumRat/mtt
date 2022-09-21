#include <mtt/editorLib/Clipboard/CECopyToClipboardOperation.h>
#include <mtt/editorLib/Objects/CEObjectSaver.h>
#include <mtt/editorLib/Objects/EnvironmentObject.h>
#include <mtt/editorLib/Objects/EnvironmentObjectFactory.h>

using namespace mtt;

CECopyToClipboardOperation::CECopyToClipboardOperation(
                                                  const std::string& mimeType) :
  CopyToClipboardOperation(mimeType)
{
  std::unique_ptr<CEObjectSaver> saver(new CEObjectSaver());
  saver->setFilenameWriteMode(ObjectSaver::WRITE_FULL_FILE_PATH);
  addSaver( environmentCategoryName,
            std::move(saver),
            std::make_unique<EnvironmentObjectFactory>());
}

bool CECopyToClipboardOperation::objectSupported(
                                            const Object& object) const noexcept
{
  return qobject_cast<const EnvironmentObject*>(&object) != nullptr;
}
