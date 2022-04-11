#include <stdexcept>

#include <QtCore/QFileInfo>
#include <QtCore/QFile>

#include <AsyncTasks/SaveEffectTask.h>
#include <Objects/ObjectSaver.h>
#include <Objects/PEEObjectFactory.h>
#include <EditorCommonData.h>
#include <ParticlesEditorScene.h>

SaveEffectTask::SaveEffectTask( const ParticlesEditorScene& scene,
                                const QString& filename,
                                EditorCommonData& commonData) :
  SaveToFileTask(QObject::tr("Saving"), filename),
  _scene(scene),
  _commonData(commonData)
{
}

void SaveEffectTask::saveData(QFile& file,
                              mtt::DataStream& stream,
                              const QFileInfo& targetFileInfo,
                              const QFileInfo& tmpFileInfo)
{
  _writeHead(file, stream);
  ObjectSaver saver;
  PEEObjectFactory factory;

  saver.saveObject( _scene.dataRoot().particleField(),
                    stream,
                    targetFileInfo.dir(),
                    factory);

  _writeModificators(stream, targetFileInfo.dir());

  saver.saveObject( _scene.dataRoot().animation(),
                    stream,
                    targetFileInfo.dir(),
                    factory);
}

void SaveEffectTask::_writeHead(QFile& file, mtt::DataStream& stream)
{
  file.write(fileHead.c_str(), fileHead.length());
  stream << fileVersion;
}

void SaveEffectTask::_writeModificators(mtt::DataStream& stream,
                                        const QDir& fileDirectory)
{
  ObjectSaver saver;
  PEEObjectFactory factory;

  uint32_t modificatorsNumber =
                          _scene.dataRoot().modificatorsGroup().childsNumber();
  stream << modificatorsNumber;
  for(uint32_t modificatorIndex = 0;
      modificatorIndex < modificatorsNumber;
      modificatorIndex++)
  {
    saver.saveObject( 
                  _scene.dataRoot().modificatorsGroup().child(modificatorIndex),
                  stream,
                  fileDirectory,
                  factory);
  }
}

void SaveEffectTask::finalizePart()
{
  _commonData.setEffectFilename(filename());
}
