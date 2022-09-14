#include <mtt/application/Scene/ObjectGroup.h>
#include <mtt/application/Scene/ObjectLoader.h>
#include <mtt/utilities/ScopedSetter.h>

using namespace mtt;

ObjectLoader::ObjectLoader() :
  _stream(nullptr),
  _objectFactory(nullptr)
{
}

void ObjectLoader::loadEmbeddedObject(Object& object,
                                      DataStream& stream,
                                      const QDir& fileDirectory,
                                      UID::ValueType mixUIDValue,
                                      const ObjectFactory& objectFactory)
{
  QString name;
  stream >> name;
  object.tryRename(name);
  _readObjectData(object, stream, fileDirectory, mixUIDValue, objectFactory);
}

void ObjectLoader::_readObjectData( Object& object,
                                      DataStream& stream,
                                      const QDir& fileDirectory,
                                      UID::ValueType mixUIDValue,
                                      const ObjectFactory& objectFactory)
{
  ScopedSetter<DataStream*> setStream(_stream, &stream);
  ScopedSetter<QDir> setFileDir(_fileDirectory, fileDirectory);
  ScopedSetter<UID::ValueType> setUidMixValue(_mixUIDValue, mixUIDValue);
  ScopedSetter<const ObjectFactory*> setObjectFactory(_objectFactory,
                                                      &objectFactory);

  process(object);
}

QString ObjectLoader::readFilename()
{
  QString relativePath;
  *_stream >> relativePath;
  if(relativePath.isEmpty()) return QString();
  QFileInfo fileInfo(_fileDirectory.absoluteFilePath(relativePath));
  return fileInfo.canonicalFilePath();
}

void ObjectLoader::visitObjectGroup(ObjectGroup& object)
{
  ObjectVisitor::visitObjectGroup(object);
  readChilds<ObjectGroup, Object>(object, true);
}

UID ObjectLoader::readUID()
{
  return _stream->readUID().mixedUID(_mixUIDValue);
}
