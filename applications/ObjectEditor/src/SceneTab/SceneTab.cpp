#include <memory>

#include <QtWidgets/QVBoxLayout>

#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/application/Widgets/PropertiesWidgets/NamePropertyWidget.h>
#include <mtt/application/Widgets/PropertiesWidgets/ReferencePropertyWidget.h>
#include <mtt/application/Widgets/ObjectsTreeView.h>
#include <mtt/application/CommonEditData.h>
#include <mtt/utilities/Abort.h>

#include <Objects/OEVisitor.h>
#include <SceneTab/AmbientLightWidget.h>
#include <SceneTab/AnimationObjectWidget.h>
#include <SceneTab/AnimationTrackWidget.h>
#include <SceneTab/BackgroundWidget.h>
#include <SceneTab/DirectLightWidget.h>
#include <SceneTab/EnvironmentModelWidget.h>
#include <SceneTab/GeometryObjectWidget.h>
#include <SceneTab/LodObjectWidget.h>
#include <SceneTab/MaterialWidget.h>
#include <SceneTab/MeshWidget.h>
#include <SceneTab/LightWidget.h>
#include <SceneTab/MovableObjectWidget.h>
#include <SceneTab/RotatableObjectWidget.h>
#include <SceneTab/ScalableObjectWidget.h>
#include <SceneTab/SceneTab.h>
#include <SceneTab/CubemapWidget.h>
#include <SceneTab/VertexDataWidget.h>
#include <SceneTab/VisiblePropertyWidget.h>
#include <EditorCommonData.h>
#include <EditorScene.h>

#include <GeneratedFiles/ui_SceneTab.h>

SceneTab::SceneTab(EditorCommonData& commonEditData) :
  _ui(new Ui::sceneTab),
  _commonEditData(commonEditData),
  _treeView(nullptr),
  _widgetsPlace(nullptr)
{
  _ui->setupUi(this);

  std::unique_ptr<SceneTreeView> treeView(new SceneTreeView(&_commonEditData));
  _treeView = treeView.get();
  _ui->objectTreePlace->layout()->addWidget(_treeView);
  treeView.release();
  _treeView->setSortFilterModel(&_sortModel);
  _treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
  _treeView->setDragEnabled(true);
  _treeView->setDragDropOverwriteMode(false);
  _treeView->setDragDropMode(QAbstractItemView::InternalMove);
  _treeView->setDefaultDropAction(Qt::MoveAction);

  _widgetsPlace = _ui->propertiesPlaceLayout;

  connect(&_commonEditData,
          &EditorCommonData::sceneChanged,
          this,
          &SceneTab::_setScene,
          Qt::DirectConnection);
  _setScene(_commonEditData.scene());

  connect(&_commonEditData,
          &EditorCommonData::selectedObjectsChanged,
          this,
          &SceneTab::_updatePropertiesWidgets,
          Qt::DirectConnection);
  _updatePropertiesWidgets();
}

SceneTab::~SceneTab() noexcept
{
  delete _ui;
}

void SceneTab::_setScene(EditorScene* scene)
{
  _treeView->setItemModel(nullptr);
  _itemModel.reset();

  if(scene != nullptr)
  {
    _itemModel.emplace(scene->root(), &_commonEditData.undoStack());
    _treeView->setItemModel(&_itemModel.value());
  }
}

struct PropertiesWidgetBuilder : public OEVisitor
{
  QVBoxLayout* widgetsLayout;
  EditorCommonData* editCommonData;
  EditorScene* scene;

  virtual void visitAmbientLightObject(mtt::AmbientLightObject& object) override
  {
    OEVisitor::visitAmbientLightObject(object);
    widgetsLayout->addWidget(
                          new AmbientLightWidget( object,
                                                  editCommonData->undoStack()));
  }

  virtual void visitAnimationObject(mtt::AnimationObject& object) override
  {
    OEVisitor::visitAnimationObject(object);

    widgetsLayout->addWidget(
          new AnimationObjectWidget(object, editCommonData->animationPlayer()));
  }

  virtual void visitAnimationTrack(mtt::AnimationTrack& object) override
  {
    OEVisitor::visitAnimationTrack(object);
    widgetsLayout->addWidget(
                      new AnimationTrackWidget( object,
                                                scene->root().skeletonGroup(),
                                                editCommonData->undoStack()));
  }

  virtual void visitBackgroundObject(mtt::BackgroundObject& object) override
  {
    OEVisitor::visitBackgroundObject(object);

    widgetsLayout->addWidget(
                    new BackgroundWidget( object, editCommonData->undoStack()));
  }

  virtual void visitBoneRefObject(BoneRefObject& object) override
  {
    OEVisitor::visitBoneRefObject(object);

    using ReferenceWidget = mtt::ReferencePropertyWidget< BoneRefObject,
                                                          mtt::SkeletonObject>;
    ReferenceWidget* referenceWidget = new ReferenceWidget(
                                                object,
                                                &BoneRefObject::bone,
                                                &BoneRefObject::setBone,
                                                &BoneRefObject::boneRefChanged,
                                                editCommonData->undoStack(),
                                                scene->root().skeletonGroup());
    widgetsLayout->addWidget(referenceWidget);
  }

  virtual void visitCubemapObject(mtt::CubemapObject& object) override
  {
    OEVisitor::visitCubemapObject(object);

    widgetsLayout->addWidget(new CubemapWidget( object,
                                                editCommonData->undoStack()));
  }


  virtual void visitDirectLightObject(mtt::DirectLightObject& object) override
  {
    OEVisitor::visitDirectLightObject(object);

    widgetsLayout->addWidget(
                    new DirectLightWidget(object, editCommonData->undoStack()));
  }

  virtual void visitDisplayedObject(mtt::DisplayedObject& object) override
  {
    OEVisitor::visitDisplayedObject(object);

    widgetsLayout->addWidget(
                new VisiblePropertyWidget(object, editCommonData->undoStack()));
  }

  virtual void visitEnvironmentModel(mtt::EnvironmentModel& object) override
  {
    OEVisitor::visitEnvironmentModel(object);

    widgetsLayout->addWidget(
              new EnvironmentModelWidget(object, editCommonData->undoStack()));
  }

  virtual void visitGeometryObject(GeometryObject& object) override
  {
    OEVisitor::visitGeometryObject(object);

    widgetsLayout->addWidget(
                        new GeometryObjectWidget( object,
                                                  scene->root().skeletonGroup(),
                                                  editCommonData->undoStack()));
  }

  virtual void visitLODObject(LODObject& object) override
  {
    OEVisitor::visitLODObject(object);

    widgetsLayout->addWidget(
                      new LODObjectWidget(object, editCommonData->undoStack()));
  }

  virtual void visitMaterialObject(MaterialObject& object) override
  {
    OEVisitor::visitMaterialObject(object);

    widgetsLayout->addWidget(
                      new MaterialWidget(object, editCommonData->undoStack()));
  }

  virtual void visitMeshObject(MeshObject& object) override
  {
    OEVisitor::visitMeshObject(object);

    widgetsLayout->addWidget( new MeshWidget( object,
                                              scene->root().materialsGroup(),
                                              editCommonData->undoStack()));
  }

  virtual void visitObject(mtt::Object& object) override
  {
    OEVisitor::visitObject(object);
    widgetsLayout->addWidget(
                      new mtt::NamePropertyWidget(object,
                                                  editCommonData->undoStack()));
  }

  virtual void visitLightObject(mtt::LightObject& object) override
  {
    OEVisitor::visitLightObject(object);

    widgetsLayout->addWidget(
                          new LightWidget(object, editCommonData->undoStack()));
  }

  virtual void visitMovableObject(mtt::MovableObject& object) override
  {
    visitObject3D(object);

    widgetsLayout->addWidget(
                  new MovableObjectWidget(object, editCommonData->undoStack()));
  }

  virtual void visitRotatableObject(mtt::RotatableObject& object) override
  {
    OEVisitor::visitRotatableObject(object);

    widgetsLayout->addWidget(
                new RotatableObjectWidget(object, editCommonData->undoStack()));
  }

  virtual void visitScalableObject(mtt::ScalableObject& object) override
  {
    OEVisitor::visitScalableObject(object);

    widgetsLayout->addWidget(
                new ScalableObjectWidget(object, editCommonData->undoStack()));
  }
};

void SceneTab::_updatePropertiesWidgets()
{
  QList<QWidget*> childWidgets = _ui->propertiesPlace->findChildren<QWidget*>(
                                                    QString(),
                                                    Qt::FindDirectChildrenOnly);
  for (QWidget* child : childWidgets) delete child;

  if(_commonEditData.selectedObjects().size() != 1) return;
  if(_commonEditData.scene() == nullptr) mtt::Abort("SceneTab::_updatePropertiesWidgets: scene is null");

  PropertiesWidgetBuilder builder;
  builder.widgetsLayout = _widgetsPlace;
  builder.editCommonData = &_commonEditData;
  builder.scene = _commonEditData.scene();
                                  
  builder.process(*_commonEditData.selectedObjects()[0]);
}
