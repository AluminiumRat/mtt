#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>
#include <QtCore/QFile>

#include <AsyncTasks/LoadEnvironmentTask.h>
#include <AsyncTasks/ObjectLoader.h>
#include <AsyncTasks/SaveEnvironmentTask.h>
#include <EditorCommonData.h>
#include <EditorScene.h>

LoadEnvironmentTask::LoadEnvironmentTask( EditorScene& scene,
                              const QString& filename,
                              EditorCommonData& commonData) :
  AbstractAsyncTask(QCoreApplication::tr("Load"),
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

void LoadEnvironmentTask::_checkHead()
{
  std::string head;
  head.resize(SaveEnvironmentTask::fileHead.size());
  _file->read(head.data(), head.size());
  if(head != SaveEnvironmentTask::fileHead) throw std::runtime_error("Invalid mmd file header");
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

  mtt::DataStream stream(&file);
  _stream = &stream;

  _checkHead();

  _background = ObjectLoader::loadObject<BackgroundObject>( false,
                                                            *_stream,
                                                            _fileDirectory,
                                                            _mixUIDValue);

  uint32_t objectsNumber = _stream->readUint32();
  for (; objectsNumber != 0; objectsNumber--)
  {
    _objects.push_back(
      ObjectLoader::loadObject<EnvironmentObject>(true,
                                                  *_stream,
                                                  _fileDirectory,
                                                  _mixUIDValue));
  }
}

void LoadEnvironmentTask::_clearScene() noexcept
{
  EnvironmentGroup& environment = _scene.root().environment();
  while (environment.childsNumber() != 0)
  {
    environment.removeChild(environment.child(environment.childsNumber() -1),
                            true);
  }
}

void LoadEnvironmentTask::finalizePart()
{
  _commonData.undoStack().clear();
  _commonData.setModelFilename("");
  _clearScene();

  try
  {
    for (std::unique_ptr<EnvironmentObject>& object : _objects)
    {
      _scene.root().environment().addChild(std::move(object));
    }

    _scene.root().changeBackground(std::move(_background));

    _commonData.setEnvironmentFilename(_filename);
  }
  catch (...)
  {
    _clearScene();
    throw;
  }
}