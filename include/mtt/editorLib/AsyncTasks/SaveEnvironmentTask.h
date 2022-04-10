#pragma once

#include <string>

#include <mtt/application/AsyncTasks/SaveToFileTask.h>

namespace mtt
{
  class BackgroundObject;
  class EnvironmentGroup;

  class SaveEnvironmentTask : public SaveToFileTask
  {
  public:
    static const inline std::string fileHead = "OEEnvironmentFile";
    static constexpr uint32_t fileVersion = 0;

  public:
    SaveEnvironmentTask(const QString& filename,
                        const EnvironmentGroup& environment,
                        const BackgroundObject& background);
    SaveEnvironmentTask(const SaveEnvironmentTask&) = delete;
    SaveEnvironmentTask& operator = (const SaveEnvironmentTask&) = delete;
    virtual ~SaveEnvironmentTask() noexcept = default;

  protected:
    virtual void saveData(QFile& file,
                          mtt::DataStream& stream,
                          const QFileInfo& targetFileInfo,
                          const QFileInfo& tmpFileInfo) override;

  private:
    void _writeHead(QFile& file, mtt::DataStream& stream);

  private:
    const EnvironmentGroup& _environment;
    const BackgroundObject& _background;
  };
}