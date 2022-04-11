#pragma once

#include <string>

#include <mtt/application/AsyncTasks/SaveToFileTask.h>

class ObjectEditorCommonData;
class ObjectEditorScene;

class SaveModelTask : public mtt::SaveToFileTask
{
public:
  static const inline std::string fileHead = "MMDModelFile";
  static constexpr uint32_t fileVersion = 0;

public:
  SaveModelTask(const ObjectEditorScene& scene,
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
  void _writeMaterials(mtt::DataStream& stream, const QDir& fileDirectory);
  void _writeSkeletons(mtt::DataStream& stream, const QDir& fileDirectory);
  void _writeGeometry(mtt::DataStream& stream, const QDir& fileDirectory);
  void _writeAnimations(mtt::DataStream& stream, const QDir& fileDirectory);

private:
  const ObjectEditorScene& _scene;
  ObjectEditorCommonData& _commonData;
};