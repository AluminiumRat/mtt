#include <stdexcept>

#include <QtCore/QByteArray>
#include <QtCore/QMimeData>
#include <QtGui/QClipboard>
#include <QtWidgets/QApplication>

#include <mtt/application/Clipboard/PasteFromClipboardOperation.h>
#include <mtt/application/EditCommands/AddObjectCommand.h>
#include <mtt/application/EditCommands/CompositeCommand.h>
#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/application/CommonEditData.h>
#include <mtt/application/DataStream.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

PasteFromClipboardOperation::PasteFromClipboardOperation(
                                  const QString& mimeType,
                                  std::unique_ptr<ObjectLoader> objectLoader,
                                  std::unique_ptr<ObjectFactory> objectFactory,
                                  CommonEditData* commonData) :
  _mimeType(mimeType),
  _objectLoader(std::move(objectLoader)),
  _objectFactory(std::move(objectFactory)),
  _commonData(commonData)
{
  if (_objectLoader == nullptr) Abort("PasteFromClipboardOperation::PasteFromClipboardOperation: objectLoader is nullptr.");
  if (_objectFactory == nullptr) Abort("PasteFromClipboardOperation::PasteFromClipboardOperation: objectFactory is nullptr.");
}

bool PasteFromClipboardOperation::mimeTypeSupported(
                                        const QString& mimeType) const noexcept
{
  return mimeType == _mimeType;
}

void PasteFromClipboardOperation::pasteFromClipboard()
{
  QClipboard* clipboard = QApplication::clipboard();
  if (clipboard == nullptr) throw std::runtime_error("Clipboard is not available.");

  const QMimeData* mimeData = clipboard->mimeData();
  if (mimeData == nullptr)
  {
    Log() << "PasteFromClipboardOperation::pasteFromClipboard: clipboard is empty.";
    return;
  }

  QByteArray data = mimeData->data(_mimeType);
  if (data.isEmpty())
  {
    Log() << "PasteFromClipboardOperation::pasteFromClipboard: clipboard is empty or unsupported mime type.";
    return;
  }

  DataStream inStream(data);
  uint32_t objectsNumber = inStream.readUint32();
  if(objectsNumber == 0) return;

  std::unique_ptr<CompositeCommand> compositeAddCommand(new CompositeCommand);
  std::vector<Object*> addedObjects;

  UID::ValueType mixUidValue = UID::randomValue();
  for (uint32_t objectIndex = 0; objectIndex < objectsNumber; objectIndex++)
  {
    bool canBeRenamed = inStream.readBool();
    std::unique_ptr<Object> newObject = _objectLoader->loadObject<Object>(
                                                              canBeRenamed,
                                                              inStream,
                                                              QDir(),
                                                              mixUidValue,
                                                              *_objectFactory);
    Object* targetGroup = prepareObjectAndGetTargetGroup( *newObject,
                                                          mixUidValue);
    if (targetGroup != nullptr)
    {
      addedObjects.push_back(newObject.get());
      compositeAddCommand->addSubcommand(
                  std::make_unique<mtt::AddObjectCommand>(std::move(newObject),
                                                          *targetGroup));
    }
  }

  if (_commonData == nullptr) compositeAddCommand->make();
  else
  {
    _commonData->undoStack().addAndMake(std::move(compositeAddCommand));
    _commonData->selectObjects(addedObjects);
  }
}
