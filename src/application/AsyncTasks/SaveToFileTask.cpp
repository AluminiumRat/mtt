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

  if (!targetFileDir.exists())
  {
    std::string errorString("SaveToFileTask: The directory does not exist: ");
    errorString += targetFileDir.canonicalPath().toLocal8Bit().data();
    throw std::runtime_error(errorString);
  }

  QString targetFilename = targetFileInfo.fileName();
  QString tmpFileName = targetFileDir.filePath(targetFilename + ".tmp");
  QFileInfo tmpFileInfo(tmpFileName);

  QFile tmpFile(tmpFileName);
  if (!tmpFile.open(QFile::WriteOnly))
  {
    std::string errorString("SaveToFileTask: Unable to create temporary file ");
    errorString += tmpFileName.toLocal8Bit().data();
    throw std::runtime_error(errorString);
  }

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
    if(!targetFile.remove())
    {
      std::string errorString("SaveToFileTask: Unable to remove old file ");
      errorString += _filename.toLocal8Bit().data();
      throw std::runtime_error(errorString);
    }
  }

  if (!tmpFile.rename(_filename))
  {
    std::string errorString("SaveToFileTask: Unable to rename file ");
    errorString += tmpFileName.toLocal8Bit().data();
    errorString += " to file ";
    errorString += _filename.toLocal8Bit().data();
    throw std::runtime_error(errorString);
  }
}
