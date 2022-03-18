#pragma once

#include <string>

#include <QtCore/QDir>
#include <QtCore/QString>

#include <mtt/application/AsyncTasks/AbstractAsyncTask.h>
#include <mtt/application/DataStream.h>

namespace mtt
{
  class Object;
}

class QFile;

class EditorCommonData;
class EditorScene;

class SaveEffectTask : public mtt::AbstractAsyncTask
{
public:
  static const inline std::string fileHead = "PEEFile";
  static constexpr uint32_t fileVersion = 0;

public:
  SaveEffectTask( const EditorScene& scene,
                  const QString& filename,
                  EditorCommonData& commonData);
  SaveEffectTask(const SaveEffectTask&) = delete;
  SaveEffectTask& operator = (const SaveEffectTask&) = delete;
  virtual ~SaveEffectTask() noexcept = default;

protected:
  virtual void asyncPart() override;
  virtual void finalizePart() override;

private:
  void _writeHead();
  void _writeModificators();

private:
  const EditorScene& _scene;
  QString _filename;
  QFile* _file;
  mtt::DataStream* _stream;
  QDir _fileDirectory;
  EditorCommonData& _commonData;
};