#pragma once

#include <memory>
#include <vector>

#include <QtCore/QDir>
#include <QtCore/QString>

#include <mtt/application/AsyncTasks/AbstractAsyncTask.h>
#include <mtt/application/DataStream.h>
#include <mtt/editorLib/Objects/EnvironmentRootObject.h>

class QFile;

namespace mtt
{
  class EditorCommonData;
  class EditorScene;

  class LoadEnvironmentTask : public mtt::AbstractAsyncTask
  {
  public:
    LoadEnvironmentTask(EditorScene& scene,
                        const QString& filename,
                        EditorCommonData& commonData);
    LoadEnvironmentTask(const LoadEnvironmentTask&) = delete;
    LoadEnvironmentTask& operator = (const LoadEnvironmentTask&) = delete;
    virtual ~LoadEnvironmentTask() noexcept = default;

  protected:
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

    std::unique_ptr<EnvironmentRootObject> _newEnvironment;

    EditorScene& _scene;
    EditorCommonData& _commonData;
  };
};