#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>
#include <QtCore/QFile>

#include <AsyncTasks/LoadModelTask.h>
#include <AsyncTasks/SaveModelTask.h>
#include <Objects/MMDObjectFactory.h>
#include <Objects/ObjectLoader.h>
#include <ObjectEditorCommonData.h>
#include <ObjectEditorScene.h>

LoadModelTask::LoadModelTask( ObjectEditorScene& scene,
                              const QString& filename,
                              ObjectEditorCommonData& commonData) :
  AbstractAsyncTask(QCoreApplication::tr("Loading"),
                    mtt::AbstractAsyncTask::DEPENDENT,
                    mtt::AbstractAsyncTask::EXPLICIT),
  _scene(scene),
  _filename(filename),
  _file(nullptr),
  _fileDirectory(QFileInfo(_filename).dir()),
  _stream(nullptr),
  _commonData(commonData),
  _mixUIDValue(mtt::UID::randomValue())
{
}

void LoadModelTask::_checkHead()
{
  std::string head;
  head.resize(SaveModelTask::fileHead.size());
  _file->read(head.data(), head.size());
  if(head != SaveModelTask::fileHead) throw std::runtime_error("LoadModelTask: Invalid mmd file header");
  uint32_t fileVersion;
  *_stream >> fileVersion;
  if(fileVersion > SaveModelTask::fileVersion)
  {
    std::string errorString("LoadModelTask: Unsupported version of mmd file: ");
    errorString += std::to_string(fileVersion);
    throw std::runtime_error(errorString);
  }
}

void LoadModelTask::asyncPart()
{
  if(!_fileDirectory.exists())
  {
    std::string errorString("LoadModelTask: The directory does not exist: ");
    errorString += _fileDirectory.canonicalPath().toLocal8Bit().data();
    throw std::runtime_error(errorString);
  }

  QFile file(_filename);
  if(!file.open(QFile::ReadOnly))
  {
    std::string errorString("LoadModelTask: Unable to open file: ");
    errorString += _filename.toLocal8Bit().data();
    throw std::runtime_error(errorString);
  }
  _file = &file;

  mtt::DataStream stream(&file);
  _stream = &stream;

  _checkHead();

  ObjectLoader loader;
  MMDObjectFactory factory;
  _newDataRoot =loader.loadObject<RootObject>(false,
                                              *_stream,
                                              _fileDirectory,
                                              _mixUIDValue,
                                              factory);
}

void LoadModelTask::finalizePart()
{
  _commonData.undoStack().clear();
  _scene.changeDataRoot(std::move(_newDataRoot));
  _commonData.setDataFilename(_filename);
}