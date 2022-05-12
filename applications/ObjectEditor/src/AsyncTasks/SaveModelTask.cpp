#include <stdexcept>

#include <QtCore/QFileInfo>
#include <QtCore/QFile>

#include <AsyncTasks/SaveModelTask.h>
#include <Objects/ObjectSaver.h>
#include <Objects/MMDObjectFactory.h>
#include <ObjectEditorCommonData.h>


SaveModelTask::SaveModelTask( const RootObject& dataRoot,
                              const QString& filename,
                              ObjectEditorCommonData& commonData) :
  SaveToFileTask(QObject::tr("Saving"), filename),
  _dataRoot(dataRoot),
  _commonData(commonData)
{
}

void SaveModelTask::saveData( QFile& file,
                              mtt::DataStream& stream,
                              const QFileInfo& targetFileInfo,
                              const QFileInfo& tmpFileInfo)
{
  _writeHead(file, stream);

  ObjectSaver saver;
  MMDObjectFactory factory;

  saver.saveObject( _dataRoot,
                    stream,
                    targetFileInfo.dir(),
                    factory);
}

void SaveModelTask::_writeHead(QFile& file, mtt::DataStream& stream)
{
  file.write(fileHead.c_str(), fileHead.length());
  stream << fileVersion;
}

void SaveModelTask::finalizePart()
{
  _commonData.setDataFilename(filename());
}
