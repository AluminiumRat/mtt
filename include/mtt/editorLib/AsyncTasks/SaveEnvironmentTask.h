#pragma once

#include <string>

#include <mtt/application/AsyncTasks/SaveToFileTask.h>

namespace mtt
{
  class EditorScene;
  class EditorCommonData;
  class EnvironmentRootObject;

  class SaveEnvironmentTask : public SaveToFileTask
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
    virtual void saveData(QFile& file,
                          mtt::DataStream& stream,
                          const QFileInfo& targetFileInfo,
                          const QFileInfo& tmpFileInfo) override;
    virtual void finalizePart() override;

  private:
    void _writeHead(QFile& file, mtt::DataStream& stream);

  private:
    const EnvironmentRootObject& _environment;
    EditorCommonData& _commonData;
  };
}