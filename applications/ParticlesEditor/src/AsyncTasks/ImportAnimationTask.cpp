#include <AsyncTasks/ImportAnimationTask.h>

#include <mtt/application/EditCommands/AddObjectCommand.h>
#include <mtt/application/EditCommands/CompositeCommand.h>
#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/editorLib/FbxAnimationImporter.h>
#include <mtt/utilities/Abort.h>

#include <Objects/ParticleAnimation.h>
#include <EditorCommonData.h>

ImportAnimationTask::ImportAnimationTask( const QString& filename,
                                          EditorCommonData& commonData) :
  AbstractAsyncTask(QObject::tr("Loading from fbx"),
                    AbstractAsyncTask::INDEPENDENT,
                    AbstractAsyncTask::EXPLICIT),
  _filename(filename),
  _commonData(commonData)
{
}

void ImportAnimationTask::asyncPart()
{
  AbstractAsyncTask::asyncPart();
  reportStage(QObject::tr("Importing fbx"));
  mtt::FbxAnimationImporter importer;
  _animation = importer.import(_filename.toUtf8().data());
}

void ImportAnimationTask::finalizePart()
{
  AbstractAsyncTask::finalizePart();

  if (_commonData.scene() == nullptr) mtt::Abort("ImportAnimationTask::finalizePart: scene is null.");
  ParticleAnimation& targetAnimation = _commonData.scene()->root().animation();

  std::vector<mtt::Object*> selection;
  std::unique_ptr<mtt::CompositeCommand> compositeCommand(
                                                    new mtt::CompositeCommand);
  while (_animation->childsNumber() != 0)
  {
    std::unique_ptr<mtt::AnimationTrack> track =
                            _animation->removeChild(_animation->child(0), true);
    selection.push_back(track.get());

    std::unique_ptr<mtt::AddObjectCommand> command(
                              new mtt::AddObjectCommand( std::move(track),
                                                        targetAnimation));
    compositeCommand->addSubcommand(std::move(command));
  }

  _commonData.undoStack().addAndMake(std::move(compositeCommand));
  _commonData.selectObjects(selection);
}
