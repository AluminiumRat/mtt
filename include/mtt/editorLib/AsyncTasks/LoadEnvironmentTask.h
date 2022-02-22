#pragma once

#include <memory>
#include <vector>

#include <QtCore/QDir>
#include <QtCore/QString>

#include <mtt/application/AsyncTasks/AbstractAsyncTask.h>
#include <mtt/application/DataStream.h>
#include <mtt/editorLib/Objects/BackgroundObject.h>
#include <mtt/editorLib/Objects/EnvironmentObject.h>

class QFile;

namespace mtt
{
  class EnvironmentGroup;
  class UndoStack;

  class LoadEnvironmentTask : public mtt::AbstractAsyncTask
  {
  public:
    LoadEnvironmentTask(const QString& filename);
    LoadEnvironmentTask(const LoadEnvironmentTask&) = delete;
    LoadEnvironmentTask& operator = (const LoadEnvironmentTask&) = delete;
    virtual ~LoadEnvironmentTask() noexcept = default;

  protected:
    inline const QString& filename() const noexcept;

    /// Called in main thread
    virtual void mergeToScene(
                std::unique_ptr<BackgroundObject> newBackground,
                std::vector<std::unique_ptr<EnvironmentObject>> newObjects) = 0;

    virtual void asyncPart() override;
    virtual void finalizePart() override;

  private:
    void _checkHead();

  private:
    QString _filename;
    QFile* _file;
    DataStream* _stream;
    QDir _fileDirectory;
    UID::ValueType _mixUIDValue;

    std::unique_ptr<BackgroundObject> _newBackground;
    std::vector<std::unique_ptr<EnvironmentObject>> _newObjects;
  };

  inline const QString& LoadEnvironmentTask::filename() const noexcept
  {
    return _filename;
  }
};