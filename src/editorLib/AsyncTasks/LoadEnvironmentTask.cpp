#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>
#include <QtCore/QFile>

#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/editorLib/AsyncTasks/SaveEnvironmentTask.h>
#include <mtt/editorLib/AsyncTasks/LoadEnvironmentTask.h>
#include <mtt/editorLib/Objects/EnvironmentObjectFactory.h>
#include <mtt/editorLib/Objects/EnvironmentGroup.h>
#include <mtt/editorLib/Objects/ObjectLoader.h>

using namespace mtt;

LoadEnvironmentTask::LoadEnvironmentTask( const QString& filename) :
  AbstractAsyncTask(QCoreApplication::tr("Environment loading"),
                    AbstractAsyncTask::DEPENDENT,
                    AbstractAsyncTask::EXPLICIT),
  _filename(filename),
  _file(nullptr),
  _stream(nullptr),
  _fileDirectory(QFileInfo(_filename).dir()),
  _mixUIDValue(UID::randomValue())
{
}

void LoadEnvironmentTask::_checkHead()
{
  std::string head;
  head.resize(SaveEnvironmentTask::fileHead.size());
  _file->read(head.data(), head.size());
  if(head != SaveEnvironmentTask::fileHead) throw std::runtime_error("Invalid enm file header");
  uint32_t fileVersion;
  *_stream >> fileVersion;
  if(fileVersion > SaveEnvironmentTask::fileVersion) throw std::runtime_error("Unsupported version of enm file");
}

void LoadEnvironmentTask::asyncPart()
{
  if(!_fileDirectory.exists()) throw std::runtime_error("The file directory does not exist");

  QFile file(_filename);
  if(!file.open(QFile::ReadOnly)) throw std::runtime_error("Unable to open model file");
  _file = &file;

  DataStream stream(&file);
  _stream = &stream;

  _checkHead();

  ObjectLoader loader;
  EnvironmentObjectFactory factory;

  _newBackground = loader.loadObject<BackgroundObject>( false,
                                                        *_stream,
                                                        _fileDirectory,
                                                        _mixUIDValue,
                                                        factory);

  uint32_t objectsNumber = _stream->readUint32();
  for (; objectsNumber != 0; objectsNumber--)
  {
    _newObjects.push_back(loader.loadObject<EnvironmentObject>( true,
                                                                *_stream,
                                                                _fileDirectory,
                                                                _mixUIDValue,
                                                                factory));
  }
}

void LoadEnvironmentTask::finalizePart()
{
  mergeToScene(std::move(_newBackground), std::move(_newObjects));
}