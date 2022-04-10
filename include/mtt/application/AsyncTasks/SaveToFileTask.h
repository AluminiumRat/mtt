#pragma once

#include <string>

#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QString>

#include <mtt/application/AsyncTasks/AbstractAsyncTask.h>
#include <mtt/application/DataStream.h>

namespace mtt
{
  class SaveToFileTask : public AbstractAsyncTask
  {
  public:
    SaveToFileTask(const QString& taskName, const QString& filename);
    SaveToFileTask(const SaveToFileTask&) = delete;
    SaveToFileTask& operator = (const SaveToFileTask&) = delete;
    virtual ~SaveToFileTask() noexcept = default;

    inline const QString& filename() const noexcept;

  protected:
    virtual void asyncPart() override;
    virtual void saveData(QFile& file,
                          mtt::DataStream& stream,
                          const QFileInfo& targetFileInfo,
                          const QFileInfo& tmpFileInfo) = 0;

  private:
    QString _filename;
  };

  inline const QString& SaveToFileTask::filename() const noexcept
  {
    return _filename;
  }
}