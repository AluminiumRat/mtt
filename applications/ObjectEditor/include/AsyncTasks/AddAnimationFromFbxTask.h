#pragma once

#include <memory>

#include <QtCore/QString>

#include <mtt/application/AsyncTasks/AbstractAsyncTask.h>
#include <mtt/editorLib/Objects/AnimationObject.h>

class EditorCommonData;

class AddAnimationFromFbxTask : public mtt::AbstractAsyncTask
{
public:
  AddAnimationFromFbxTask(const QString& filename, EditorCommonData& commonData);
  AddAnimationFromFbxTask(const AddAnimationFromFbxTask&) = delete;
  AddAnimationFromFbxTask& operator = (const AddAnimationFromFbxTask&) = delete;
  virtual ~AddAnimationFromFbxTask() noexcept = default;

protected:
  virtual void asyncPart() override;
  virtual void finalizePart() override;

private:
  QString _filename;
  EditorCommonData& _commonData;

  std::unique_ptr<mtt::AnimationObject> _animation;
};
