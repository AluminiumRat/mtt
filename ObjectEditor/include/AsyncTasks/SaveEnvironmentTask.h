#pragma once

#include <string>

#include <QtCore/QDir>
#include <QtCore/QString>

#include <mtt/Application/AsyncTasks/AbstractAsyncTask.h>
#include <mtt/Application/DataStream.h>

namespace mtt
{
  class Object;
}

class QFile;

class EditorCommonData;
class EditorScene;

class SaveEnvironmentTask : public mtt::AbstractAsyncTask
{
public:
  static const inline std::string fileHead = "OEEnvironmentFile";
  static constexpr uint32_t fileVersion = 0;

public:
  SaveEnvironmentTask(const EditorScene& scene,
                      const QString& filename,
                      EditorCommonData& commonData);
  SaveEnvironmentTask(const SaveEnvironmentTask&) = delete;
  SaveEnvironmentTask& operator = (const SaveEnvironmentTask&) = delete;
  virtual ~SaveEnvironmentTask() noexcept = default;

protected:
  virtual void asyncPart() override;
  virtual void finalizePart() override;

private:
  void _writeHead();

private:
  const EditorScene& _scene;
  QString _filename;
  QFile* _file;
  mtt::DataStream* _stream;
  QDir _fileDirectory;
  EditorCommonData& _commonData;
};