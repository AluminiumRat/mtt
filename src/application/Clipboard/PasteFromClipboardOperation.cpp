#include <stdexcept>

#include <QtCore/QMimeData>
#include <QtGui/QClipboard>
#include <QtWidgets/QApplication>

#include <mtt/application/Clipboard/CopyToClipboardOperation.h>
#include <mtt/application/Clipboard/PasteFromClipboardOperation.h>
#include <mtt/application/EditCommands/AddObjectCommand.h>
#include <mtt/application/EditCommands/CompositeCommand.h>
#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/application/CommonEditData.h>
#include <mtt/application/DataStream.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

PasteFromClipboardOperation::PasteFromClipboardOperation(
                                                  CommonEditData* commonData) :
  _commonData(commonData)
{
}

void PasteFromClipboardOperation::addLoader(
                                  std::string categoryName,
                                  std::unique_ptr<ObjectLoader> objectLoader,
                                  std::unique_ptr<ObjectFactory> objectFactory)
{
  if (objectLoader == nullptr) Abort("PasteFromClipboardOperation::addLoader: objectLoader is nullptr.");
  if (objectFactory == nullptr) Abort("PasteFromClipboardOperation::addLoader: objectFactory is nullptr.");
  for (const LoaderRecord& record : _loaders)
  {
    if(record.categoryName == categoryName) Abort("PasteFromClipboardOperation::addLoader: loader with this categoryName is already registered.");
  }

  LoaderRecord newRecord;
  newRecord.categoryName = categoryName;
  newRecord.loader = std::move(objectLoader);
  newRecord.factory = std::move(objectFactory);
  _loaders.push_back(std::move(newRecord));
}

QByteArray PasteFromClipboardOperation::_getClipboardData()
{
  QClipboard* clipboard = QApplication::clipboard();
  if (clipboard == nullptr) throw std::runtime_error("Clipboard is not available.");

  const QMimeData* mimeData = clipboard->mimeData();
  if (mimeData == nullptr)
  {
    Log() << "PasteFromClipboardOperation::_getClipboardData: clipboard is empty.";
    return QByteArray();
  }

  return mimeData->data(CopyToClipboardOperation::mimeType);
}

PasteFromClipboardOperation::LoaderRecord& 
                  PasteFromClipboardOperation::_getLoader(DataStream& inStream)
{
  QString categoryName;
  inStream >> categoryName;
  std::string category = categoryName.toStdString();

  LoaderRecord* loaderRecord = nullptr;
  for (LoaderRecord& record : _loaders)
  {
    if (record.categoryName == category)
    {
      loaderRecord = &record;
      break;
    }
  }
  if (loaderRecord == nullptr)
  {
    std::string errorString("Unknown object category: ");
    errorString += category;
    throw std::runtime_error(errorString);
  }
  return *loaderRecord;
}

void PasteFromClipboardOperation::pasteFromClipboard()
{
  QByteArray data = _getClipboardData();
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
    LoaderRecord& loaderRecord = _getLoader(inStream);

    bool canBeRenamed = inStream.readBool();
    std::unique_ptr<Object> newObject =
                      loaderRecord.loader->loadObject<Object>(
                                                        canBeRenamed,
                                                        inStream,
                                                        QDir(),
                                                        mixUidValue,
                                                        *loaderRecord.factory);
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
