#include <QtCore/QFileInfo>
#include <QtCore/QFile>

#include <AsyncTasks/LoadEffectTask.h>
#include <AsyncTasks/SaveEffectTask.h>
#include <Objects/PEEObjectFactory.h>
#include <Objects/ObjectLoader.h>
#include <EditorCommonData.h>
#include <EditorScene.h>

LoadEffectTask::LoadEffectTask( EditorScene& scene,
                                const QString& filename,
                                EditorCommonData& commonData) :
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

  _field = loader.loadObject<ParticleField>(false,
                                            *_stream,
                                            _fileDirectory,
                                            _mixUIDValue,
                                            factory);
  _loadModificators();

  _animation = loader.loadObject<ParticleAnimation>(false,
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

void LoadEffectTask::_loadModificators()
{
  ObjectLoader loader;
  PEEObjectFactory factory;

  uint32_t modificatorsNumber = _stream->readUint32();
  for (; modificatorsNumber != 0; modificatorsNumber--)
  {
    _modificators.push_back(
                          loader.loadObject<HierarhicalObject>( true,
                                                                *_stream,
                                                                _fileDirectory,
                                                                _mixUIDValue,
                                                                factory));
  }
}

void LoadEffectTask::finalizePart()
{
  _commonData.undoStack().clear();
  _commonData.setEffectFilename("");
  _clearScene();

  try
  {
    _scene.root().changeParticleField(std::move(_field));
    for (std::unique_ptr<HierarhicalObject>& modificator : _modificators)
    {
      _scene.root().modificatorsGroup().addChild(std::move(modificator));
    }
    _scene.root().changeAnimation(std::move(_animation));

    _commonData.setEffectFilename(_filename);
  }
  catch (...)
  {
    _clearScene();
    throw;
  }
}

void LoadEffectTask::_clearScene() noexcept
{
  ModificatorGroup& modificators = _scene.root().modificatorsGroup();
  while (modificators.childsNumber() != 0)
  {
    modificators.removeChild(modificators.child(0), true);
  }
}
