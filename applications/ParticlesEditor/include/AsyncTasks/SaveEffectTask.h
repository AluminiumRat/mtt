#pragma once

#include <string>

#include <mtt/application/AsyncTasks/SaveToFileTask.h>

class ParticlesEditorCommonData;
class RootObject;

class SaveEffectTask : public mtt::SaveToFileTask
{
public:
  static const inline std::string fileHead = "PEEFile";
  static constexpr uint32_t fileVersion = 0;

public:
  SaveEffectTask( const RootObject& data,
                  const QString& filename,
                  ParticlesEditorCommonData& commonData);
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

private:
  const RootObject& _data;
  ParticlesEditorCommonData& _commonData;
};