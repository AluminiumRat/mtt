#include <mtt/application/EditCommands/AddObjectCommand.h>
#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/application/CommonEditData.h>
#include <mtt/editorLib/AsyncTasks/AddAnimationFromFbxTask.h>
#include <mtt/editorLib/Objects/AnimationGroup.h>
#include <mtt/editorLib/FbxAnimationImporter.h>

using namespace mtt;

AddAnimationFromFbxTask::AddAnimationFromFbxTask( const QString& filename,
                                                  AnimationGroup& targetGroup,
                                                  Object* skeletonSearchGroup,
                                                  CommonEditData& commonData) :
  AbstractAsyncTask(QObject::tr("Load from fbx"),
                    AbstractAsyncTask::INDEPENDENT,
                    AbstractAsyncTask::EXPLICIT),
  _filename(filename),
  _targetGroup(targetGroup),
  _skeletonSearchGroup(skeletonSearchGroup),
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

static SkeletonObject* findSkeleton( const QString& name,
                                          Object& searchArea)
{
  SkeletonObject* skeleton = qobject_cast<SkeletonObject*>(&searchArea);
  if(skeleton != nullptr && skeleton->name() == name) return skeleton;
  for(size_t childIndex = 0;
      childIndex < searchArea.subobjectNumber();
      childIndex++)
  {
    SkeletonObject* childSkeleton = findSkeleton(
                                              name,
                                              searchArea.subobject(childIndex));
    if(childSkeleton != nullptr) return childSkeleton;
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

  if(_skeletonSearchGroup != nullptr)
  {
    for(size_t trackIndex = 0;
        trackIndex < _animation->childsNumber();
        trackIndex++)
    {
      AnimationTrack& track = _animation->child(trackIndex);
      SkeletonObject* skeleton = findSkeleton(track.name(),
                                              *_skeletonSearchGroup);
      if(skeleton != nullptr) track.setSkeleton(skeleton);
    }
  }

  std::unique_ptr<AddObjectCommand> command(
                                    new AddObjectCommand( std::move(_animation),
                                                          _targetGroup));
  _commonData.undoStack().addAndMake(std::move(command));
  _commonData.selectObjects({ animationPtr });
}
