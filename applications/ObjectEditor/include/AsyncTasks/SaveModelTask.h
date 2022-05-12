#pragma once

#include <string>

#include <mtt/application/AsyncTasks/SaveToFileTask.h>

class ObjectEditorCommonData;
class RootObject;

class SaveModelTask : public mtt::SaveToFileTask
{
public:
  static const inline std::string fileHead = "MMDModelFile";
  static constexpr uint32_t fileVersion = 0;

public:
  SaveModelTask(const RootObject& dataRoot,
                const QString& filename,
                ObjectEditorCommonData& commonData);
  SaveModelTask(const SaveModelTask&) = delete;
  SaveModelTask& operator = (const SaveModelTask&) = delete;
  virtual ~SaveModelTask() noexcept = default;

protected:
  virtual void saveData(QFile& file,
                        mtt::DataStream& stream,
                        const QFileInfo& targetFileInfo,
                        const QFileInfo& tmpFileInfo) override;
  virtual void finalizePart() override;

private:
  void _writeHead(QFile& file, mtt::DataStream& stream);

private:
  const RootObject& _dataRoot;
  ObjectEditorCommonData& _commonData;
};