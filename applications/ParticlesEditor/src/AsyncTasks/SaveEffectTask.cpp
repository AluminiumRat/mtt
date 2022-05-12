#include <stdexcept>

#include <QtCore/QFileInfo>
#include <QtCore/QFile>

#include <AsyncTasks/SaveEffectTask.h>
#include <Objects/ObjectSaver.h>
#include <Objects/PEEObjectFactory.h>
#include <Objects/RootObject.h>
#include <ParticlesEditorCommonData.h>

SaveEffectTask::SaveEffectTask( const RootObject& data,
                                const QString& filename,
                                ParticlesEditorCommonData& commonData) :
  SaveToFileTask(QObject::tr("Saving"), filename),
  _data(data),
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
  saver.saveObject(_data, stream, targetFileInfo.dir(), factory);
}

void SaveEffectTask::_writeHead(QFile& file, mtt::DataStream& stream)
{
  file.write(fileHead.c_str(), fileHead.length());
  stream << fileVersion;
}

void SaveEffectTask::finalizePart()
{
  _commonData.setDataFilename(filename());
}
