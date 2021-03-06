#include <vector>

#include <QtCore/QFileInfo>
#include <QtCore/QObject>

#include <mtt/application/EditCommands/AddObjectCommand.h>
#include <mtt/application/EditCommands/CompositeCommand.h>

#include <AsyncTasks/AddModelFromFbxTask.h>
#include <Objects/LODObject.h>
#include <ObjectEditorCommonData.h>

AddModelFromFbxTask::AddModelFromFbxTask(
                  const QString& filename,
                  const mtt::BaseFbxImporter::MaterialOptions& materialOptions,
                  ObjectEditorCommonData& commonData) :
  AbstractAsyncTask(QObject::tr("Load from fbx"),
                    AbstractAsyncTask::INDEPENDENT,
                    AbstractAsyncTask::EXPLICIT),
  _filename(filename),
  _materialOptions(materialOptions),
  _commonData(commonData)
{
}

void AddModelFromFbxTask::asyncPart()
{
  AbstractAsyncTask::asyncPart();

  reportStage(QObject::tr("Importing fbx"));
  FbxDrawableImporter importer;
  _data = std::move(importer.import(_filename.toUtf8().data(),
                                    _materialOptions));
}

void AddModelFromFbxTask::finalizePart()
{
  AbstractAsyncTask::finalizePart();

  ObjectEditorScene* scene = _commonData.scene();
  if(scene == nullptr) return;

  QFileInfo fileInfo(_filename);

  std::vector<mtt::Object*> toSelect;
  std::unique_ptr<mtt::CompositeCommand> compositeCommand(
                                                  new mtt::CompositeCommand);
  
  std::unique_ptr<LODObject> lod(new LODObject(fileInfo.fileName(), true));
  toSelect.push_back(lod.get());
  for(std::unique_ptr<MeshObject>& mesh : _data.drawables)
  {
    lod->addChild(std::move(mesh));
  }
  std::unique_ptr<mtt::AddObjectCommand> addLodCommand(
                            new mtt::AddObjectCommand(
                                            std::move(lod),
                                            scene->dataRoot().geometryGroup()));
  compositeCommand->addSubcommand(std::move(addLodCommand));

  for(std::unique_ptr<MaterialObject>& material : _data.materials)
  {
    toSelect.push_back(material.get());
    std::unique_ptr<mtt::AddObjectCommand> subCommand(
                              new mtt::AddObjectCommand(
                                          std::move(material),
                                          scene->dataRoot().materialsGroup()));
    compositeCommand->addSubcommand(std::move(subCommand));
  }

  for(std::unique_ptr<mtt::SkeletonObject>& skeleton : _data.skeleton)
  {
    toSelect.push_back(skeleton.get());
    std::unique_ptr<mtt::AddObjectCommand> subCommand(
                              new mtt::AddObjectCommand(
                                            std::move(skeleton),
                                            scene->dataRoot().skeletonGroup()));
    compositeCommand->addSubcommand(std::move(subCommand));
  }

  _commonData.undoStack().addAndMake(std::move(compositeCommand));
  _commonData.selectObjects(toSelect);
}
