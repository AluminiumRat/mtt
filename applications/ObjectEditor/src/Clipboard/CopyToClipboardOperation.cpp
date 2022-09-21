#include <mtt/editorLib/Objects/AnimationObject.h>
#include <mtt/editorLib/Objects/AnimationTrack.h>
#include <mtt/editorLib/Objects/SkeletonObject.h>

#include <Clipboard/CopyToClipboardOperation.h>
#include <Objects/LODObject.h>
#include <Objects/MaterialObject.h>
#include <Objects/MeshObject.h>
#include <Objects/ObjectSaver.h>
#include <Objects/MMDObjectFactory.h>

CopyToClipboardOperation::CopyToClipboardOperation()
{
  std::unique_ptr<ObjectSaver> saver(new ObjectSaver());
  saver->setFilenameWriteMode(ObjectSaver::WRITE_FULL_FILE_PATH);
  addSaver( objectEditorCategoryName,
            std::move(saver),
            std::make_unique<MMDObjectFactory>());
}

bool CopyToClipboardOperation::objectSupported(
                                      const mtt::Object& object) const noexcept
{
  if (qobject_cast<const mtt::AnimationObject*>(&object) != nullptr ||
      qobject_cast<const mtt::AnimationTrack*>(&object) != nullptr ||
      qobject_cast<const LODObject*>(&object) != nullptr ||
      qobject_cast<const MaterialObject*>(&object) != nullptr ||
      qobject_cast<const MeshObject*>(&object) != nullptr ||
      qobject_cast<const mtt::SkeletonObject*>(&object) != nullptr)
  {
    return true;
  }

  return CECopyToClipboardOperation::objectSupported(object);
}
