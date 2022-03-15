#include <QtWidgets/QVBoxLayout>

#include <mtt/application/Widgets/PropertiesWidgets/ReferencePropertyWidget.h>

#include <SceneTab/GeometryObjectWidget.h>
#include <SceneTab/LodObjectWidget.h>
#include <SceneTab/MaterialWidget.h>
#include <SceneTab/MeshWidget.h>
#include <SceneTab/PropertiesWidgetFactory.h>
#include <SceneTab/VertexDataWidget.h>
#include <EditorCommonData.h>
#include <EditorScene.h>

PropertiesWidgetFactory::PropertiesWidgetFactory(
                                        QVBoxLayout& widgetsLayout,
                                        EditorCommonData& commonData) :
  OEVisitorT( widgetsLayout,
              commonData,
              &commonData.scene()->root().skeletonGroup(),
              &commonData.animationPlayer()),
  _commonData(commonData),
  _scene(*_commonData.scene())
{
}

void PropertiesWidgetFactory::visitBoneRefObject(BoneRefObject& object)
{
  OEVisitorT::visitBoneRefObject(object);

  using ReferenceWidget = mtt::ReferencePropertyWidget<
                                                      BoneRefObject,
                                                      mtt::SkeletonObject,
                                                      &BoneRefObject::boneRef>;
  ReferenceWidget* referenceWidget = new ReferenceWidget(
                                              object,
                                              &BoneRefObject::boneRefChanged,
                                              _commonData.undoStack(),
                                              _scene.root().skeletonGroup());
  widgetsLayout().addWidget(referenceWidget);
}

void PropertiesWidgetFactory::visitGeometryObject(GeometryObject& object)
{
  OEVisitorT::visitGeometryObject(object);

  widgetsLayout().addWidget(
                        new GeometryObjectWidget( object,
                                                  _scene.root().skeletonGroup(),
                                                  _commonData.undoStack()));
}

void PropertiesWidgetFactory::visitLODObject(LODObject& object)
{
  OEVisitorT::visitLODObject(object);

  widgetsLayout().addWidget(
                          new LODObjectWidget(object, _commonData.undoStack()));
}

void PropertiesWidgetFactory::visitMaterialObject(MaterialObject& object)
{
  OEVisitorT::visitMaterialObject(object);

  widgetsLayout().addWidget(
                          new MaterialWidget(object, _commonData.undoStack()));
}

void PropertiesWidgetFactory::visitMeshObject(MeshObject& object)
{
  OEVisitorT::visitMeshObject(object);

  widgetsLayout().addWidget( new MeshWidget(object,
                                            _scene.root().materialsGroup(),
                                            _commonData.undoStack()));
}
