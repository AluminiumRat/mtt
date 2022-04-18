#include <mtt/application/EditCommands/AddObjectCommand.h>
#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/application/CommonEditData.h>
#include <mtt/editorLib/AsyncTasks/AddAnimationFromFbxTask.h>
#include <mtt/editorLib/Objects/AnimationGroup.h>
#include <mtt/editorLib/Objects/PositionAnimator.h>
#include <mtt/editorLib/FbxAnimationImporter.h>

using namespace mtt;

AddAnimationFromFbxTask::AddAnimationFromFbxTask( const QString& filename,
                                                  AnimationGroup& targetGroup,
                                                  Object* targetSearchGroup,
                                                  CommonEditData& commonData) :
  AbstractAsyncTask(QObject::tr("Loading from fbx"),
                    AbstractAsyncTask::INDEPENDENT,
                    AbstractAsyncTask::EXPLICIT),
  _filename(filename),
  _targetGroup(targetGroup),
  _targetSearchGroup(targetSearchGroup),
  _commonData(commonData)
{
}

void AddAnimationFromFbxTask::asyncPart()
{
  AbstractAsyncTask::asyncPart();

  reportStage(QObject::tr("Importing fbx"));
  FbxAnimationImporter importer;
  _animation = importer.import(_filename.toUtf8().data());
}

static ScalableObject* findTarget(const QString& name, Object& searchArea)
{
  ScalableObject* scalableObject = qobject_cast<ScalableObject*>(&searchArea);
  if(scalableObject != nullptr && scalableObject->name() == name)
  {
    return scalableObject;
  }

  for(size_t childIndex = 0;
      childIndex < searchArea.subobjectNumber();
      childIndex++)
  {
    ScalableObject* childTarget = findTarget(
                                              name,
                                              searchArea.subobject(childIndex));
    if(childTarget != nullptr) return childTarget;
  }
  return nullptr;
}

void AddAnimationFromFbxTask::finalizePart()
{
  AbstractAsyncTask::finalizePart();

  AnimationObject* animationPtr = _animation.get();
  if(_animation->childsNumber() == 0)
  {
    throw std::runtime_error("Animation not found in fbx file");
  }

  if(_targetSearchGroup != nullptr)
  {
    for(size_t trackIndex = 0;
        trackIndex < _animation->childsNumber();
        trackIndex++)
    {
      AnimationTrack& track = _animation->child(trackIndex);
      PositionAnimator* positionAnimator =
                                        qobject_cast<PositionAnimator*>(&track);
      if(positionAnimator != nullptr)
      {
        ScalableObject* target = findTarget(track.name(), *_targetSearchGroup);
        if(target != nullptr) positionAnimator->targetRef().set(target);
      }
    }
  }

  std::unique_ptr<AddObjectCommand> command(
                                    new AddObjectCommand( std::move(_animation),
                                                          _targetGroup));
  _commonData.undoStack().addAndMake(std::move(command));
  _commonData.selectObjects({ animationPtr });
}
