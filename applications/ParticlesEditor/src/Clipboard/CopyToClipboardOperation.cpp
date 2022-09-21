#include <mtt/editorLib/Objects/AnimationTrack.h>

#include <Clipboard/CopyToClipboardOperation.h>
#include <Objects/HierarhicalObject.h>
#include <Objects/ObjectSaver.h>
#include <Objects/PEEObjectFactory.h>

CopyToClipboardOperation::CopyToClipboardOperation()
{
  std::unique_ptr<ObjectSaver> saver(new ObjectSaver());
  saver->setFilenameWriteMode(ObjectSaver::WRITE_FULL_FILE_PATH);
  addSaver( particlesCategoryName,
            std::move(saver),
            std::make_unique<PEEObjectFactory>());
}

bool CopyToClipboardOperation::objectSupported(
                                      const mtt::Object& object) const noexcept
{
  if (qobject_cast<const mtt::AnimationTrack*>(&object) != nullptr) return true;
  if (qobject_cast<const HierarhicalObject*>(&object) != nullptr) return true;
  return CECopyToClipboardOperation::objectSupported(object);
}
