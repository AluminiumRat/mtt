#pragma once

#include <mtt/editorLib/SceneTab/PropertiesWidgetFactory.h>

#include <Objects/OEVisitor.h>

class EditorCommonData;
class ObjectEditorScene;

class PropertiesWidgetFactory : public OEVisitorT<mtt::PropertiesWidgetFactory>
{
public:
  PropertiesWidgetFactory(QVBoxLayout& widgetsLayout,
                          EditorCommonData& commonData);
  PropertiesWidgetFactory(const PropertiesWidgetFactory&) = delete;
  PropertiesWidgetFactory& operator = (
                                      const PropertiesWidgetFactory&) = delete;
  virtual ~PropertiesWidgetFactory() noexcept = default;

  virtual void visitBoneRefObject(BoneRefObject& object) override;
  virtual void visitGeometryObject(GeometryObject& object) override;
  virtual void visitLODObject(LODObject& object) override;
  virtual void visitMaterialObject(MaterialObject& object) override;
  virtual void visitMeshObject(MeshObject& object) override;

private:
  EditorCommonData& _commonData;
  ObjectEditorScene& _scene;
};