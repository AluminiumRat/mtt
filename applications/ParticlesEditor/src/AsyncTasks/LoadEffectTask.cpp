#include <QtCore/QFileInfo>
#include <QtCore/QFile>

#include <AsyncTasks/LoadEffectTask.h>
#include <AsyncTasks/SaveEffectTask.h>
#include <Objects/PEEObjectFactory.h>
#include <Objects/ObjectLoader.h>
#include <ParticlesEditorCommonData.h>
#include <ParticlesEditorScene.h>

LoadEffectTask::LoadEffectTask( ParticlesEditorScene& scene,
                                const QString& filename,
                                ParticlesEditorCommonData& commonData) :
  AbstractAsyncTask(QObject::tr("Loading"),
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

void LoadEffectTask::asyncPart()
{
  if(!_fileDirectory.exists()) throw std::runtime_error("The file directory does not exist");

  QFile file(_filename);
  if(!file.open(QFile::ReadOnly)) throw std::runtime_error("Unable to open effect file");
  _file = &file;

  mtt::DataStream stream(&file);
  _stream = &stream;

  _checkHead();

  ObjectLoader loader;
  PEEObjectFactory factory;

  _newData = loader.loadObject<RootObject>( false,
                                            *_stream,
                                            _fileDirectory,
                                            _mixUIDValue,
                                            factory);
}

void LoadEffectTask::_checkHead()
{
  std::string head;
  head.resize(SaveEffectTask::fileHead.size());
  _file->read(head.data(), head.size());
  if(head != SaveEffectTask::fileHead) throw std::runtime_error("Invalid pee file header");
  uint32_t fileVersion;
  *_stream >> fileVersion;
  if(fileVersion > SaveEffectTask::fileVersion) throw std::runtime_error("Unsupported version of pee file");
}

void LoadEffectTask::finalizePart()
{
  _commonData.undoStack().clear();
  _scene.changeDataRoot(std::move(_newData));
  _commonData.setDataFilename(_filename);
}

