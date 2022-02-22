#pragma once

#include <string>

#include <QtCore/QDir>
#include <QtCore/QString>

#include <mtt/application/AsyncTasks/AbstractAsyncTask.h>
#include <mtt/application/DataStream.h>

class QFile;

namespace mtt
{
  class BackgroundObject;
  class EnvironmentGroup;

  class SaveEnvironmentTask : public AbstractAsyncTask
  {
  public:
    static const inline std::string fileHead = "OEEnvironmentFile";
    static constexpr uint32_t fileVersion = 0;

  public:
    SaveEnvironmentTask(const QString& filename,
                        const EnvironmentGroup& environment,
                        const BackgroundObject& background);
    SaveEnvironmentTask(const SaveEnvironmentTask&) = delete;
    SaveEnvironmentTask& operator = (const SaveEnvironmentTask&) = delete;
    virtual ~SaveEnvironmentTask() noexcept = default;

  protected:
    inline const QString& filename() const noexcept;

    virtual void asyncPart() override;

  private:
    void _writeHead();

  private:
    const EnvironmentGroup& _environment;
    const BackgroundObject& _background;

    QString _filename;
    QFile* _file;
    DataStream* _stream;
    QDir _fileDirectory;
  };

  inline const QString& SaveEnvironmentTask::filename() const noexcept
  {
    return _filename;
  }
}