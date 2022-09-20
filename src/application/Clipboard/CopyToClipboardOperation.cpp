#include <limits>
#include <stdexcept>

#include <QtCore/QByteArray>
#include <QtCore/QMimeData>
#include <QtGui/QClipboard>
#include <QtWidgets/QApplication>

#include <mtt/application/Clipboard/CopyToClipboardOperation.h>
#include <mtt/application/DataStream.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

void CopyToClipboardOperation::addSaver(
                                  std::string categoryName,
                                  std::unique_ptr<ObjectSaver> objectSaver,
                                  std::unique_ptr<ObjectFactory> objectFactory)
{
  if (objectSaver == nullptr) Abort("CopyToClipboardOperation::addSaver: objectSaver is nullptr.");
  if (objectFactory == nullptr) Abort("CopyToClipboardOperation::addSaver: objectFactory is nullptr.");
  for (const SaverRecord& record : _savers)
  {
    if(record.categoryName == categoryName) Abort("CopyToClipboardOperation::addSaver: saver with this categoryName is already registered.");
  }

  SaverRecord newRecord;
  newRecord.categoryName = categoryName;
  newRecord.saver =  std::move(objectSaver);
  newRecord.factory = std::move(objectFactory);
  _savers.push_back(std::move(newRecord));
}

std::set<Object*> CopyToClipboardOperation::_clearObjects(
                                            const std::vector<Object*>& objects)
{
  std::set<Object*> result(objects.begin(), objects.end());

  std::set<Object*>::iterator iObject = result.begin();

  while (iObject != result.end())
  {
    bool toDelete = false;
    Object* parent = (*iObject)->parent();
    while (parent != nullptr)
    {
      if (result.count(parent) != 0)
      {
        toDelete = true;
        break;
      }
      parent = parent->parent();
    }

    if (toDelete) iObject = result.erase(iObject);
    else iObject++;
  }

  return result;
}

void CopyToClipboardOperation::copyToClipboard(
                                            const std::vector<Object*>& objects)
{
  std::set<Object*> clearedObjects = _clearObjects(objects);
  if(clearedObjects.empty()) return;
  if(clearedObjects.size() > std::numeric_limits<uint32_t>::max()) throw std::runtime_error("Too many objects to copy to clipboard.d");

  QByteArray data;
  DataStream outStream(&data, QIODevice::WriteOnly);

  outStream<<uint32_t(clearedObjects.size());
  for (Object* object : clearedObjects)
  {
    bool objectSaved = false;
    for (const SaverRecord& record : _savers)
    {
      if (record.factory->getTypeIndex(*object) != ObjectFactory::notIndex)
      {
        outStream << QString(record.categoryName.c_str());
        outStream << object->canBeRenamed();
        record.saver->saveObject(*object, outStream, QDir(), *record.factory);
        objectSaved = true;
        break;
      }
    }

    if (!objectSaved)
    {
      Log() << "CopyToClipboardOperation::copyToClipboard: Unable to serialize class " << object->metaObject()->className();
      Abort("CopyToClipboardOperation::copyToClipboard: Unable to serialize object.");
    }
  }

  std::unique_ptr<QMimeData> mimeData(new QMimeData);
  mimeData->setData(mimeType, data);

  QClipboard* clipboard = QApplication::clipboard();
  if(clipboard == nullptr) throw std::runtime_error("Clipboard is not available.");
  clipboard->setMimeData(mimeData.release());
}
