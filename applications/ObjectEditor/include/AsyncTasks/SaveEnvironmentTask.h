#pragma once

#include <mtt/editorLib/AsyncTasks/SaveEnvironmentTask.h>

class ObjectEditorCommonData;
class ObjectEditorScene;

class SaveEnvironmentTask : public mtt::SaveEnvironmentTask
{
public:
  SaveEnvironmentTask(const ObjectEditorScene& scene,
                      const QString& filename,
                      ObjectEditorCommonData& commonData);
  SaveEnvironmentTask(const SaveEnvironmentTask&) = delete;
  SaveEnvironmentTask& operator = (const SaveEnvironmentTask&) = delete;
  virtual ~SaveEnvironmentTask() noexcept = default;

protected:
  virtual void finalizePart() override;

private:
  ObjectEditorCommonData& _commonData;
};