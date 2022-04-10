#include <stdexcept>

#include <QtCore/QDir>

#include <mtt/application/AsyncTasks/SaveToFileTask.h>

using namespace mtt;

SaveToFileTask::SaveToFileTask( const QString& taskName,
                                const QString& filename) :
  AbstractAsyncTask(taskName,
                    mtt::AbstractAsyncTask::DEPENDENT,
                    mtt::AbstractAsyncTask::EXPLICIT),
  _filename(filename)
{
}

void SaveToFileTask::asyncPart()
{
  QFileInfo targetFileInfo(_filename);
  QDir targetFileDir = targetFileInfo.dir();

  if (!targetFileDir.exists()) throw std::runtime_error("The file directory does not exist");

  QString targetFilename = targetFileInfo.fileName();
  QString tmpFileName = targetFileDir.filePath(targetFilename + ".tmp");
  QFileInfo tmpFileInfo(tmpFileName);

  QFile tmpFile(tmpFileName);
  if (!tmpFile.open(QFile::WriteOnly)) throw std::runtime_error("Unable to open file");

  try
  {
    mtt::DataStream stream(&tmpFile);
    saveData(tmpFile, stream, targetFileInfo, tmpFileInfo);
  }
  catch (...)
  {
    tmpFile.close();
    tmpFile.remove();
    throw;
  }

  tmpFile.close();

  if(targetFileInfo.exists())
  {
    QFile targetFile(_filename);
    if(!targetFile.remove()) throw std::runtime_error("Unable to remove old file");
  }

  tmpFile.rename(_filename);
}
