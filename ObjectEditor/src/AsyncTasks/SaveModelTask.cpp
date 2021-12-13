#include <stdexcept>

#include <QtCore/QFileInfo>
#include <QtCore/QFile>

#include <AsyncTasks/ObjectDataSaver.h>
#include <AsyncTasks/SaveModelTask.h>
#include <EditorCommonData.h>
#include <EditorScene.h>


SaveModelTask::SaveModelTask( const EditorScene& scene,
                              const QString& filename,
                              EditorCommonData& commonData) :
  AbstractAsyncTask("Save",
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

void SaveModelTask::asyncPart()
{
  if(!_fileDirectory.exists()) throw std::runtime_error("The file directory does not exist");

  QFile file(_filename);
  if(!file.open(QFile::WriteOnly)) throw std::runtime_error("Unable to open model file");
  _file = &file;

  mtt::DataStream stream(&file);
  _stream = &stream;

  _writeHead();
  _writeMaterials();
  _writeSkeletons();
  _writeGeometry();
  _writeAnimations();
}

void SaveModelTask::_writeHead()
{
  _file->write(fileHead.c_str(), fileHead.length());
  *_stream << fileVersion;
}

void SaveModelTask::_writeMaterials()
{
  uint32_t materialsNumber = _scene.root().materialsGroup().childsNumber();
  *_stream << materialsNumber;
  for(uint32_t materialIndex = 0;
      materialIndex < materialsNumber;
      materialIndex++)
  {
    ObjectDataSaver::saveObject(
                            _scene.root().materialsGroup().child(materialIndex),
                            *_stream,
                            _fileDirectory);
  }
}

void SaveModelTask::_writeSkeletons()
{
  uint32_t skeletonsNumber = _scene.root().skeletonGroup().childsNumber();
  *_stream << skeletonsNumber;
  for(uint32_t skeletonIndex = 0;
      skeletonIndex < skeletonsNumber;
      skeletonIndex++)
  {
    ObjectDataSaver::saveObject(
                            _scene.root().skeletonGroup().child(skeletonIndex),
                            *_stream,
                            _fileDirectory);
  }
}

void SaveModelTask::_writeGeometry()
{
  uint32_t lodsNumber = _scene.root().geometryGroup().childsNumber();
  *_stream << lodsNumber;
  for(uint32_t lodIndex = 0;
      lodIndex < lodsNumber;
      lodIndex++)
  {
    ObjectDataSaver::saveObject(_scene.root().geometryGroup().child(lodIndex),
                                *_stream,
                                _fileDirectory);
  }
}

void SaveModelTask::_writeAnimations()
{
  uint32_t animationsNumber = _scene.root().animationGroup().childsNumber();
  *_stream << animationsNumber;
  for(uint32_t animationIndex = 0;
      animationIndex < animationsNumber;
      animationIndex++)
  {
    ObjectDataSaver::saveObject(
                          _scene.root().animationGroup().child(animationIndex),
                          *_stream,
                          _fileDirectory);
  }
}

void SaveModelTask::finalizePart()
{
  _commonData.setModelFilename(_filename);
}
