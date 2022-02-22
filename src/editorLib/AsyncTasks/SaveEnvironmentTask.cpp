#include <stdexcept>

#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>
#include <QtCore/QFile>

#include <mtt/editorLib/AsyncTasks/SaveEnvironmentTask.h>
#include <mtt/editorLib/Objects/EnvironmentObjectFactory.h>
#include <mtt/editorLib/Objects/ObjectSaver.h>

using namespace mtt;

SaveEnvironmentTask::SaveEnvironmentTask( const QString& filename,
                                          const EnvironmentGroup& environment,
                                          const BackgroundObject& background) :
  AbstractAsyncTask(QCoreApplication::tr("Environment saving"),
                    AbstractAsyncTask::DEPENDENT,
                    AbstractAsyncTask::EXPLICIT),
  _environment(environment),
  _background(background),
  _filename(filename),
  _file(nullptr),
  _fileDirectory(QFileInfo(_filename).dir()),
  _stream(nullptr)
{
}

void SaveEnvironmentTask::asyncPart()
{
  if(!_fileDirectory.exists()) throw std::runtime_error("The file directory does not exist");

  QFile file(_filename);
  if(!file.open(QFile::WriteOnly)) throw std::runtime_error("Unable to open model file");
  _file = &file;

  DataStream stream(&file);
  _stream = &stream;

  _writeHead();

  ObjectSaver saver;
  EnvironmentObjectFactory factory;

  saver.saveObject(_background, *_stream, _fileDirectory, factory);

  uint32_t objectsNumber = uint32_t(_environment.childsNumber());
  *_stream << objectsNumber;
  for(uint32_t objectIndex = 0;
      objectIndex < objectsNumber;
      objectIndex++)
  {
    saver.saveObject( _environment.child(objectIndex),
                      *_stream,
                      _fileDirectory,
                      factory);
  }
}

void SaveEnvironmentTask::_writeHead()
{
  _file->write(fileHead.c_str(), fileHead.length());
  *_stream << fileVersion;
}
