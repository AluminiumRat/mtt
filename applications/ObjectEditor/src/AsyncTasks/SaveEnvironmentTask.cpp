#include <stdexcept>

#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>
#include <QtCore/QFile>

#include <AsyncTasks/ObjectSaver.h>
#include <AsyncTasks/SaveEnvironmentTask.h>
#include <EditorCommonData.h>
#include <EditorScene.h>


SaveEnvironmentTask::SaveEnvironmentTask( const EditorScene& scene,
                              const QString& filename,
                              EditorCommonData& commonData) :
  AbstractAsyncTask(QCoreApplication::tr("Save"),
                    mtt::AbstractAsyncTask::DEPENDENT,
                    mtt::AbstractAsyncTask::EXPLICIT),
  _scene(scene),
  _filename(filename),
  _file(nullptr),
  _fileDirectory(QFileInfo(_filename).dir()),
  _stream(nullptr),
  _commonData(commonData)
{
}

void SaveEnvironmentTask::asyncPart()
{
  if(!_fileDirectory.exists()) throw std::runtime_error("The file directory does not exist");

  QFile file(_filename);
  if(!file.open(QFile::WriteOnly)) throw std::runtime_error("Unable to open model file");
  _file = &file;

  mtt::DataStream stream(&file);
  _stream = &stream;

  _writeHead();

  ObjectSaver::saveObject(_scene.root().background(), *_stream, _fileDirectory);

  uint32_t objectsNumber = _scene.root().environment().childsNumber();
  *_stream << objectsNumber;
  for(uint32_t objectIndex = 0;
      objectIndex < objectsNumber;
      objectIndex++)
  {
    ObjectSaver::saveObject(_scene.root().environment().child(objectIndex),
                            *_stream,
                            _fileDirectory);
  }
}

void SaveEnvironmentTask::_writeHead()
{
  _file->write(fileHead.c_str(), fileHead.length());
  *_stream << fileVersion;
}

void SaveEnvironmentTask::finalizePart()
{
  _commonData.setEnvironmentFilename(_filename);
}
