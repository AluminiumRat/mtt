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

class SaveModelTask : public mtt::AbstractAsyncTask
{
public:
  static const inline std::string fileHead = "MMDModelFile";
  static constexpr uint32_t fileVersion = 0;

public:
  SaveModelTask(const EditorScene& scene,
                const QString& filename,
                EditorCommonData& commonData);
  SaveModelTask(const SaveModelTask&) = delete;
  SaveModelTask& operator = (const SaveModelTask&) = delete;
  virtual ~SaveModelTask() noexcept = default;

protected:
  virtual void asyncPart() override;
  virtual void finalizePart() override;

private:
  void _writeHead();
  void _writeMaterials();
  void _writeSkeletons();
  void _writeGeometry();
  void _writeAnimations();

private:
  const EditorScene& _scene;
  QString _filename;
  QFile* _file;
  mtt::DataStream* _stream;
  QDir _fileDirectory;
  EditorCommonData& _commonData;
};