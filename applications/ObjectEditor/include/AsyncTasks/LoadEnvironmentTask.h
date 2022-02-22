#pragma once

#include <mtt/editorLib/AsyncTasks/LoadEnvironmentTask.h>

class EditorCommonData;
class EditorScene;

class LoadEnvironmentTask : public mtt::LoadEnvironmentTask
{
public:
  LoadEnvironmentTask(EditorScene& scene,
                      const QString& filename,
                      EditorCommonData& commonData);
  LoadEnvironmentTask(const LoadEnvironmentTask&) = delete;
  LoadEnvironmentTask& operator = (const LoadEnvironmentTask&) = delete;
  virtual ~LoadEnvironmentTask() noexcept = default;

protected:
  virtual void mergeToScene(
      std::unique_ptr<mtt::BackgroundObject> newBackground,
      std::vector<std::unique_ptr<mtt::EnvironmentObject>> newObjects) override;

private:
  void _clearScene() noexcept;

private:
  EditorScene& _scene;
  EditorCommonData& _commonData;
};