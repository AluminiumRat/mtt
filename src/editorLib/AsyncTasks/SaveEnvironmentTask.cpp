#include <stdexcept>

#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>
#include <QtCore/QFile>

#include <mtt/editorLib/AsyncTasks/SaveEnvironmentTask.h>
#include <mtt/editorLib/Objects/EnvironmentObjectFactory.h>
#include <mtt/editorLib/Objects/ObjectSaver.h>

using namespace mtt;

SaveEnvironmentTask::SaveEnvironmentTask( const QString& filename,
                                          const EnvironmentGroup& environment,
                                          const BackgroundObject& background) :
  SaveToFileTask(QCoreApplication::tr("Environment saving"), filename),
  _environment(environment),
  _background(background)
{
}

void SaveEnvironmentTask::saveData( QFile& file,
                                    mtt::DataStream& stream,
                                    const QFileInfo& targetFileInfo,
                                    const QFileInfo& tmpFileInfo)
{
  _writeHead(file, stream);

  ObjectSaver saver;
  EnvironmentObjectFactory factory;

  saver.saveObject(_background, stream, targetFileInfo.dir(), factory);

  uint32_t objectsNumber = uint32_t(_environment.childsNumber());
  stream << objectsNumber;
  for(uint32_t objectIndex = 0;
      objectIndex < objectsNumber;
      objectIndex++)
  {
    saver.saveObject( _environment.child(objectIndex),
                      stream,
                      targetFileInfo.dir(),
                      factory);
  }
}

void SaveEnvironmentTask::_writeHead(QFile& file, mtt::DataStream& stream)
{
  file.write(fileHead.c_str(), fileHead.length());
  stream << fileVersion;
}
