#pragma once

#include <mtt/editorLib/AsyncTasks/LoadEnvironmentTask.h>

class ObjectEditorCommonData;
class ObjectEditorScene;

class LoadEnvironmentTask : public mtt::LoadEnvironmentTask
{
public:
  LoadEnvironmentTask(ObjectEditorScene& scene,
                      const QString& filename,
                      ObjectEditorCommonData& commonData);
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
  ObjectEditorScene& _scene;
  ObjectEditorCommonData& _commonData;
};