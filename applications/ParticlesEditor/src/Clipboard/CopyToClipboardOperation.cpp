#include <mtt/editorLib/Objects/AnimationTrack.h>

#include <Clipboard/CopyToClipboardOperation.h>
#include <Objects/HierarhicalObject.h>
#include <Objects/ObjectSaver.h>
#include <Objects/PEEObjectFactory.h>

CopyToClipboardOperation::CopyToClipboardOperation()
{
  addSaver( particlesCategoryName,
            std::make_unique<ObjectSaver>(),
            std::make_unique<PEEObjectFactory>());
}

bool CopyToClipboardOperation::objectSupported(
                                      const mtt::Object& object) const noexcept
{
  if (qobject_cast<const mtt::AnimationTrack*>(&object) != nullptr) return true;
  if (qobject_cast<const HierarhicalObject*>(&object) != nullptr) return true;
  return CECopyToClipboardOperation::objectSupported(object);
}
