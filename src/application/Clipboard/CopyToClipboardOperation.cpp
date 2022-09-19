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

CopyToClipboardOperation::CopyToClipboardOperation(
                                const QString& mimeType,
                                std::unique_ptr<ObjectSaver> objectSaver,
                                std::unique_ptr<ObjectFactory> objectFactory) :
  _mimeType(mimeType),
  _objectSaver(std::move(objectSaver)),
  _objectFactory(std::move(objectFactory))
{
  if(_objectSaver == nullptr) Abort("CopyToClipboardOperation::CopyToClipboardOperation: objectSaver is nullptr.");
  if (_objectFactory == nullptr) Abort("CopyToClipboardOperation::CopyToClipboardOperation: objectFactory is nullptr.");
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
    outStream << object->canBeRenamed();
    _objectSaver->saveObject(*object, outStream, QDir(), *_objectFactory);
  }

  std::unique_ptr<QMimeData> mimeData(new QMimeData);
  mimeData->setData(_mimeType, data);

  QClipboard* clipboard = QApplication::clipboard();
  if(clipboard == nullptr) throw std::runtime_error("Clipboard is not available.");
  clipboard->setMimeData(mimeData.release());
}
