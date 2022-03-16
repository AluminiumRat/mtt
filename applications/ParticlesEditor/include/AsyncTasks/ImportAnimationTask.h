#pragma once

#include <memory>

#include <QtCore/QString>

#include <mtt/application/AsyncTasks/AbstractAsyncTask.h>
#include <mtt/editorLib/Objects/AnimationObject.h>

class EditorCommonData;

class ImportAnimationTask : public mtt::AbstractAsyncTask
{
public:
  ImportAnimationTask(const QString& filename,
                      EditorCommonData& commonData);
  ImportAnimationTask(const ImportAnimationTask&) = delete;
  ImportAnimationTask& operator = ( const ImportAnimationTask&) = delete;
  virtual ~ImportAnimationTask() noexcept = default;

protected:
  virtual void asyncPart() override;
  virtual void finalizePart() override;

private:
  QString _filename;
  EditorCommonData& _commonData;
  std::unique_ptr<mtt::AnimationObject> _animation;
};
