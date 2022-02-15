#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>
#include <QtCore/QFile>

#include <AsyncTasks/LoadModelTask.h>
#include <AsyncTasks/ObjectLoader.h>
#include <AsyncTasks/SaveModelTask.h>
#include <EditorCommonData.h>
#include <EditorScene.h>

LoadModelTask::LoadModelTask( EditorScene& scene,
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

void LoadModelTask::_checkHead()
{
  std::string head;
  head.resize(SaveModelTask::fileHead.size());
  _file->read(head.data(), head.size());
  if(head != SaveModelTask::fileHead) throw std::runtime_error("Invalid mmd file header");
  uint32_t fileVersion;
  *_stream >> fileVersion;
  if(fileVersion > SaveModelTask::fileVersion) throw std::runtime_error("Unsupported version of mmd file");
}

void LoadModelTask::_loadMaterials()
{
  uint32_t materialsNumber = _stream->readUint32();
  for (; materialsNumber != 0; materialsNumber--)
  {
    _materials.push_back(
                    ObjectLoader::loadObject<MaterialObject>( true,
                                                              *_stream,
                                                              _fileDirectory,
                                                              _mixUIDValue));
  }
}

void LoadModelTask::_loadSkeletons()
{
  uint32_t skeletonsNumber = _stream->readUint32();
  for (; skeletonsNumber != 0; skeletonsNumber--)
  {
    _skeletons.push_back(
                  ObjectLoader::loadObject<mtt::SkeletonObject>(true,
                                                                *_stream,
                                                                _fileDirectory,
                                                                _mixUIDValue));
  }
}

void LoadModelTask::_loadGeometry()
{
  uint32_t lodsNumber = _stream->readUint32();
  for (; lodsNumber != 0; lodsNumber--)
  {
    _lods.push_back(ObjectLoader::loadObject<LODObject>(true,
                                                        *_stream,
                                                        _fileDirectory,
                                                        _mixUIDValue));
  }
}

void LoadModelTask::_loadAnimations()
{
  uint32_t animationsNumber = _stream->readUint32();
  for (; animationsNumber != 0; animationsNumber--)
  {
    _animations.push_back(
              ObjectLoader::loadObject<mtt::AnimationObject>( true,
                                                              *_stream,
                                                              _fileDirectory,
                                                              _mixUIDValue));
  }
}

void LoadModelTask::asyncPart()
{
  if(!_fileDirectory.exists()) throw std::runtime_error("The file directory does not exist");

  QFile file(_filename);
  if(!file.open(QFile::ReadOnly)) throw std::runtime_error("Unable to open model file");
  _file = &file;

  mtt::DataStream stream(&file);
  _stream = &stream;

  _checkHead();
  _loadMaterials();
  _loadSkeletons();
  _loadGeometry();
  _loadAnimations();
}

void LoadModelTask::_clearScene() noexcept
{
  mtt::AnimationGroup& animations = _scene.root().animationGroup();
  while (animations.childsNumber() != 0)
  {
    animations.removeChild( animations.child(animations.childsNumber() -1),
                            true);
  }

  GeometryGroup& geometry = _scene.root().geometryGroup();
  while (geometry.childsNumber() != 0)
  {
    geometry.removeChild( geometry.child(geometry.childsNumber() -1),
                          true);
  }

  MaterialsGroup& materials = _scene.root().materialsGroup();
  while (materials.childsNumber() != 0)
  {
    materials.removeChild(materials.child(materials.childsNumber() -1),
                          true);
  }

  mtt::SkeletonGroup& skeletons = _scene.root().skeletonGroup();
  while (skeletons.childsNumber() != 0)
  {
    skeletons.removeChild(skeletons.child(skeletons.childsNumber() -1),
                          true);
  }
}

void LoadModelTask::finalizePart()
{
  _commonData.undoStack().clear();
  _commonData.setModelFilename("");
  _clearScene();

  try
  {
    for (std::unique_ptr<mtt::AnimationObject>& animation : _animations)
    {
      _scene.root().animationGroup().addChild(std::move(animation));
    }

    for (std::unique_ptr<LODObject>& lod : _lods)
    {
      _scene.root().geometryGroup().addChild(std::move(lod));
    }

    for (std::unique_ptr<MaterialObject>& material : _materials)
    {
      _scene.root().materialsGroup().addChild(std::move(material));
    }

    for (std::unique_ptr<mtt::SkeletonObject>& skeleton : _skeletons)
    {
      _scene.root().skeletonGroup().addChild(std::move(skeleton));
    }

    _commonData.setModelFilename(_filename);
  }
  catch (...)
  {
    _clearScene();
    throw;
  }
}