#pragma once

#include <mtt/editorLib/AsyncTasks/LoadEnvironmentTask.h>

class EditorCommonData;
class ParticlesEditorScene;

class LoadEnvironmentTask : public mtt::LoadEnvironmentTask
{
public:
  LoadEnvironmentTask(ParticlesEditorScene& scene,
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
  ParticlesEditorScene& _scene;
  EditorCommonData& _commonData;
};