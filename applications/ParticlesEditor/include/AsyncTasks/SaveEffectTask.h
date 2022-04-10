#pragma once

#include <string>

#include <mtt/application/AsyncTasks/SaveToFileTask.h>

class EditorCommonData;
class EditorScene;

class SaveEffectTask : public mtt::SaveToFileTask
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
  virtual void saveData(QFile& file,
                        mtt::DataStream& stream,
                        const QFileInfo& targetFileInfo,
                        const QFileInfo& tmpFileInfo) override;
  virtual void finalizePart() override;

private:
  void _writeHead(QFile& file, mtt::DataStream& stream);
  void _writeModificators(mtt::DataStream& stream, const QDir& fileDirectory);

private:
  const EditorScene& _scene;
  EditorCommonData& _commonData;
};