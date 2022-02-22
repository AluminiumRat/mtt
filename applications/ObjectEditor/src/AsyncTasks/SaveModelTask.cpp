#include <stdexcept>

#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>
#include <QtCore/QFile>

#include <AsyncTasks/SaveModelTask.h>
#include <Objects/ObjectSaver.h>
#include <Objects/MMDObjectFactory.h>
#include <EditorCommonData.h>
#include <EditorScene.h>


SaveModelTask::SaveModelTask( const EditorScene& scene,
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
  ObjectSaver saver;
  MMDObjectFactory factory;

  uint32_t materialsNumber = _scene.root().materialsGroup().childsNumber();
  *_stream << materialsNumber;
  for(uint32_t materialIndex = 0;
      materialIndex < materialsNumber;
      materialIndex++)
  {
    saver.saveObject( _scene.root().materialsGroup().child(materialIndex),
                      *_stream,
                      _fileDirectory,
                      factory);
  }
}

void SaveModelTask::_writeSkeletons()
{
  ObjectSaver saver;
  MMDObjectFactory factory;

  uint32_t skeletonsNumber = _scene.root().skeletonGroup().childsNumber();
  *_stream << skeletonsNumber;
  for(uint32_t skeletonIndex = 0;
      skeletonIndex < skeletonsNumber;
      skeletonIndex++)
  {
    saver.saveObject( _scene.root().skeletonGroup().child(skeletonIndex),
                      *_stream,
                      _fileDirectory,
                      factory);
  }
}

void SaveModelTask::_writeGeometry()
{
  ObjectSaver saver;
  MMDObjectFactory factory;

  uint32_t lodsNumber = _scene.root().geometryGroup().childsNumber();
  *_stream << lodsNumber;
  for(uint32_t lodIndex = 0;
      lodIndex < lodsNumber;
      lodIndex++)
  {
    saver.saveObject( _scene.root().geometryGroup().child(lodIndex),
                      *_stream,
                      _fileDirectory,
                      factory);
  }
}

void SaveModelTask::_writeAnimations()
{
  ObjectSaver saver;
  MMDObjectFactory factory;

  uint32_t animationsNumber = _scene.root().animationGroup().childsNumber();
  *_stream << animationsNumber;
  for(uint32_t animationIndex = 0;
      animationIndex < animationsNumber;
      animationIndex++)
  {
    saver.saveObject( _scene.root().animationGroup().child(animationIndex),
                      *_stream,
                      _fileDirectory,
                      factory);
  }
}

void SaveModelTask::finalizePart()
{
  _commonData.setModelFilename(_filename);
}
