#include <stdexcept>

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
  SaveToFileTask(QObject::tr("Saving"), filename),
  _scene(scene),
  _commonData(commonData)
{
}

void SaveModelTask::saveData( QFile& file,
                              mtt::DataStream& stream,
                              const QFileInfo& targetFileInfo,
                              const QFileInfo& tmpFileInfo)
{
  _writeHead(file, stream);
  _writeMaterials(stream, targetFileInfo.dir());
  _writeSkeletons(stream, targetFileInfo.dir());
  _writeGeometry(stream, targetFileInfo.dir());
  _writeAnimations(stream, targetFileInfo.dir());
}

void SaveModelTask::_writeHead(QFile& file, mtt::DataStream& stream)
{
  file.write(fileHead.c_str(), fileHead.length());
  stream << fileVersion;
}

void SaveModelTask::_writeMaterials(mtt::DataStream& stream,
                                    const QDir& fileDirectory)
{
  ObjectSaver saver;
  MMDObjectFactory factory;

  uint32_t materialsNumber = _scene.root().materialsGroup().childsNumber();
  stream << materialsNumber;
  for(uint32_t materialIndex = 0;
      materialIndex < materialsNumber;
      materialIndex++)
  {
    saver.saveObject( _scene.root().materialsGroup().child(materialIndex),
                      stream,
                      fileDirectory,
                      factory);
  }
}

void SaveModelTask::_writeSkeletons(mtt::DataStream& stream,
                                    const QDir& fileDirectory)
{
  ObjectSaver saver;
  MMDObjectFactory factory;

  uint32_t skeletonsNumber = _scene.root().skeletonGroup().childsNumber();
  stream << skeletonsNumber;
  for(uint32_t skeletonIndex = 0;
      skeletonIndex < skeletonsNumber;
      skeletonIndex++)
  {
    saver.saveObject( _scene.root().skeletonGroup().child(skeletonIndex),
                      stream,
                      fileDirectory,
                      factory);
  }
}

void SaveModelTask::_writeGeometry( mtt::DataStream& stream,
                                    const QDir& fileDirectory)
{
  ObjectSaver saver;
  MMDObjectFactory factory;

  uint32_t lodsNumber = _scene.root().geometryGroup().childsNumber();
  stream << lodsNumber;
  for(uint32_t lodIndex = 0;
      lodIndex < lodsNumber;
      lodIndex++)
  {
    saver.saveObject( _scene.root().geometryGroup().child(lodIndex),
                      stream,
                      fileDirectory,
                      factory);
  }
}

void SaveModelTask::_writeAnimations( mtt::DataStream& stream,
                                      const QDir& fileDirectory)
{
  ObjectSaver saver;
  MMDObjectFactory factory;

  uint32_t animationsNumber = _scene.root().animationGroup().childsNumber();
  stream << animationsNumber;
  for(uint32_t animationIndex = 0;
      animationIndex < animationsNumber;
      animationIndex++)
  {
    saver.saveObject( _scene.root().animationGroup().child(animationIndex),
                      stream,
                      fileDirectory,
                      factory);
  }
}

void SaveModelTask::finalizePart()
{
  _commonData.setModelFilename(filename());
}
