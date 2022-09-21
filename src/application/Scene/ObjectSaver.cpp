#include <stdexcept>

#include <mtt/application/Scene/Object.h>
#include <mtt/application/Scene/ObjectGroup.h>
#include <mtt/application/Scene/ObjectSaver.h>
#include <mtt/utilities/ScopedSetter.h>

using namespace mtt;

ObjectSaver::ObjectSaver() :
  _stream(nullptr),
  _objectFactory(nullptr),
  _filenameWriteMode(WRITE_RELATIVE_FILE_PATH)
{
}

void ObjectSaver::saveObject( const mtt::Object& object,
                              mtt::DataStream& stream,
                              const QDir& fileDirectory,
                              const ObjectFactory& objectFactory)
{
  uint16_t typeIndex = objectFactory.getTypeIndex(object);
  if (typeIndex == ObjectFactory::notIndex)
  {
    std::string errorString("ObjectSaver: Unsupported object type: ");
    errorString += object.metaObject()->className();
    throw std::runtime_error(errorString);
  }
  stream << typeIndex;
  stream << object.id();
  saveObjectData(object, stream, fileDirectory, objectFactory);
}

void ObjectSaver::saveObjectData( const Object& object,
                                  DataStream& stream,
                                  const QDir& fileDirectory,
                                  const ObjectFactory& objectFactory)
{
  ScopedSetter<DataStream*> setStream(_stream, &stream);
  ScopedSetter<QDir> setFileDir(_fileDirectory, fileDirectory);
  ScopedSetter<const ObjectFactory*> setObjectFactory(_objectFactory,
                                                      &objectFactory);
  process(object);
}

void ObjectSaver::writeFilename(const QString& filename)
{
  if(_filenameWriteMode == WRITE_RELATIVE_FILE_PATH)
  {
    *_stream << _fileDirectory.relativeFilePath(filename);
  }
  else *_stream << filename;
}

void ObjectSaver::writeChilds(const Object& parent)
{
  uint32_t objectsNumber = uint32_t(parent.subobjectNumber());
  stream() << objectsNumber;
  for(uint32_t objectIndex = 0;
      objectIndex < objectsNumber;
      objectIndex++)
  {
    saveObject( parent.subobject(objectIndex),
                stream(),
                fileDirectory(),
                objectFactory());
  }
}

void ObjectSaver::visitConstObjectGroup(const ObjectGroup& object)
{
  ObjectVisitor::visitConstObjectGroup(object);
  writeChilds(object);
}

void ObjectSaver::visitConstObject(const mtt::Object& object)
{
  *_stream << object.name();
}
