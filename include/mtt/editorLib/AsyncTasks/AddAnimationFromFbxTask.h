#pragma once

#include <memory>

#include <QtCore/QString>

#include <mtt/application/AsyncTasks/AbstractAsyncTask.h>
#include <mtt/editorLib/Objects/AnimationObject.h>

namespace mtt
{
  class CommonEditData;
  class AnimationGroup;

  class AddAnimationFromFbxTask : public mtt::AbstractAsyncTask
  {
  public:
    AddAnimationFromFbxTask(const QString& filename,
                            AnimationGroup& targetGroup,
                            Object* skeletonSearchGroup,
                            CommonEditData& commonData);
    AddAnimationFromFbxTask(const AddAnimationFromFbxTask&) = delete;
    AddAnimationFromFbxTask& operator = (
                                      const AddAnimationFromFbxTask&) = delete;
    virtual ~AddAnimationFromFbxTask() noexcept = default;

  protected:
    virtual void asyncPart() override;
    virtual void finalizePart() override;

  private:
    QString _filename;
    AnimationGroup& _targetGroup;
    Object* _skeletonSearchGroup;
    CommonEditData& _commonData;

    std::unique_ptr<AnimationObject> _animation;
  };
}