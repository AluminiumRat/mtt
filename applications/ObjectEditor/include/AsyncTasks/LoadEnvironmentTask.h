#pragma once

#include <memory>
#include <vector>

#include <QtCore/QDir>
#include <QtCore/QString>

#include <mtt/application/AsyncTasks/AbstractAsyncTask.h>
#include <mtt/application/DataStream.h>

#include <Objects/BackgroundObject.h>
#include <Objects/EnvironmentObject.h>

class QFile;

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
  void _clearScene() noexcept;

private:
  EditorScene& _scene;
  QString _filename;
  QFile* _file;
  mtt::DataStream* _stream;
  QDir _fileDirectory;
  EditorCommonData& _commonData;
  mtt::UID::ValueType _mixUIDValue;

  std::unique_ptr<BackgroundObject> _background;
  std::vector<std::unique_ptr<EnvironmentObject>> _objects;
};