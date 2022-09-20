#include <Clipboard/CopyToClipboardOperation.h>
#include <Objects/ObjectSaver.h>
#include <Objects/PEEObjectFactory.h>

CopyToClipboardOperation::CopyToClipboardOperation()
{
  addSaver( particlesCategoryName,
            std::make_unique<ObjectSaver>(),
            std::make_unique<PEEObjectFactory>());
}
