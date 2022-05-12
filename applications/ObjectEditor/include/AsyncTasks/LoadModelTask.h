#pragma once

#include <memory>

#include <QtCore/QDir>
#include <QtCore/QString>

#include <mtt/application/AsyncTasks/AbstractAsyncTask.h>
#include <mtt/application/DataStream.h>

#include <Objects/RootObject.h>

class QFile;

class ObjectEditorCommonData;
class ObjectEditorScene;

class LoadModelTask : public mtt::AbstractAsyncTask
{
public:
  LoadModelTask(ObjectEditorScene& scene,
                const QString& filename,
                ObjectEditorCommonData& commonData);
  LoadModelTask(const LoadModelTask&) = delete;
  LoadModelTask& operator = (const LoadModelTask&) = delete;
  virtual ~LoadModelTask() noexcept = default;

protected:
  virtual void asyncPart() override;
  virtual void finalizePart() override;

private:
  void _checkHead();

private:
  ObjectEditorScene& _scene;
  QString _filename;
  QFile* _file;
  mtt::DataStream* _stream;
  QDir _fileDirectory;
  ObjectEditorCommonData& _commonData;
  mtt::UID::ValueType _mixUIDValue;

  std::unique_ptr<RootObject> _newDataRoot;
};