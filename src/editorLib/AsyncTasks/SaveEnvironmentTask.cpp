#include <stdexcept>

#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>
#include <QtCore/QFile>

#include <mtt/editorLib/AsyncTasks/SaveEnvironmentTask.h>
#include <mtt/editorLib/Objects/EnvironmentObjectFactory.h>
#include <mtt/editorLib/Objects/EnvironmentRootObject.h>
#include <mtt/editorLib/Objects/ObjectSaver.h>
#include <mtt/editorLib/EditorCommonData.h>
#include <mtt/editorLib/EditorScene.h>

using namespace mtt;

SaveEnvironmentTask::SaveEnvironmentTask( const EditorScene& scene,
                                          const QString& filename,
                                          EditorCommonData& commonData) :
  SaveToFileTask(QCoreApplication::tr("Environment saving"), filename),
  _environment(scene.environmentRoot()),
  _commonData(commonData)
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

  saver.saveObjectData( _environment.background(),
                        stream,
                        targetFileInfo.dir(),
                        factory);
  saver.saveObjectData( _environment.objectsGroup(),
                        stream,
                        targetFileInfo.dir(),
                        factory);
}

void SaveEnvironmentTask::_writeHead(QFile& file, mtt::DataStream& stream)
{
  file.write(fileHead.c_str(), fileHead.length());
  stream << fileVersion;
}

void SaveEnvironmentTask::finalizePart()
{
  _commonData.setEnvironmentFilename(filename());
}
