#include <mtt/application/EditCommands/AddObjectCommand.h>

#include <AsyncTasks/AddAnimationFromFbxTask.h>
#include <EditorCommonData.h>
#include <FbxAnimationImporter.h>

AddAnimationFromFbxTask::AddAnimationFromFbxTask(
                                                const QString& filename,
                                                EditorCommonData& commonData) :
  AbstractAsyncTask(QObject::tr("Load from fbx"),
                    AbstractAsyncTask::INDEPENDENT,
                    AbstractAsyncTask::EXPLICIT),
  _filename(filename),
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

static mtt::SkeletonObject* findSkeleton( const QString& name,
                                          mtt::Object& searchArea)
{
  mtt::SkeletonObject* skeleton =
                                qobject_cast<mtt::SkeletonObject*>(&searchArea);
  if(skeleton != nullptr && skeleton->name() == name) return skeleton;
  for(size_t childIndex = 0;
      childIndex < searchArea.subobjectNumber();
      childIndex++)
  {
    mtt::SkeletonObject* childSkeleton = findSkeleton(
                                              name,
                                              searchArea.subobject(childIndex));
    if(childSkeleton != nullptr) return childSkeleton;
  }
  return nullptr;
}

void AddAnimationFromFbxTask::finalizePart()
{
  AbstractAsyncTask::finalizePart();

  EditorScene* scene = _commonData.scene();
  if (scene == nullptr) return;

  mtt::AnimationObject* animationPtr = _animation.get();
  if(_animation->childsNumber() == 0)
  {
    throw std::runtime_error("Animation not found in fbx file");
  }

  for(size_t trackIndex = 0;
      trackIndex < _animation->childsNumber();
      trackIndex++)
  {
    mtt::AnimationTrack& track = _animation->child(trackIndex);
    mtt::SkeletonObject* skeleton = findSkeleton(
                                                track.name(),
                                                scene->root().skeletonGroup());
    if(skeleton != nullptr) track.setSkeleton(skeleton);
  }

  std::unique_ptr<mtt::AddObjectCommand> command(
                                new mtt::AddObjectCommand(
                                            std::move(_animation),
                                            scene->root().animationGroup()));
  _commonData.undoStack().addAndMake(std::move(command));
  
  _commonData.selectObjects({ animationPtr });
}
