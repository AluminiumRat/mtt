#include <QtWidgets/QVBoxLayout>

#include <mtt/application/Widgets/PropertiesWidgets/ReferencePropertyWidget.h>

#include <PropertiesWidget/GeometryObjectWidget.h>
#include <PropertiesWidget/LodObjectWidget.h>
#include <PropertiesWidget/MaterialWidget.h>
#include <PropertiesWidget/MeshWidget.h>
#include <PropertiesWidget/PropertiesWidgetFactory.h>
#include <PropertiesWidget/VertexDataWidget.h>
#include <ObjectEditorCommonData.h>
#include <ObjectEditorScene.h>

PropertiesWidgetFactory::PropertiesWidgetFactory(
                                        QVBoxLayout& widgetsLayout,
                                        ObjectEditorCommonData& commonData) :
  OEVisitorT( widgetsLayout,
              commonData,
              &commonData.scene()->dataRoot().skeletonGroup(),
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
                                            _scene.dataRoot().skeletonGroup());
  widgetsLayout().addWidget(referenceWidget);
}

void PropertiesWidgetFactory::visitGeometryObject(GeometryObject& object)
{
  OEVisitorT::visitGeometryObject(object);

  widgetsLayout().addWidget(
                    new GeometryObjectWidget( object,
                                              _scene.dataRoot().skeletonGroup(),
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
                                            _scene.dataRoot().materialsGroup(),
                                            _commonData.undoStack()));
}
