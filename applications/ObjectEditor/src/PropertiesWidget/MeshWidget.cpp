#include <mtt/application/Widgets/PropertiesWidgets/ReferencePropertyWidget.h>

#include <PropertiesWidget/MaterialWidget.h>
#include <PropertiesWidget/MeshWidget.h>
#include <PropertiesWidget/VertexDataWidget.h>

#include <GeneratedFiles/ui_MeshWidget.h>

MeshWidget::MeshWidget( MeshObject& object,
                        mtt::Object& materialsGroup,
                        mtt::UndoStack& undoStack) :
  _ui(new Ui::MeshWidget),
  _object(object),
  _undoStack(undoStack),
  _materialWidget(nullptr)
{
  _ui->setupUi(this);

  VertexDataWidget* vertexDataWidget = new VertexDataWidget(object);
  _ui->vertexDataLayout->addWidget(vertexDataWidget);

  using ReferenceWidget = mtt::ReferencePropertyWidget<
                                                      MeshObject,
                                                      MaterialObject,
                                                      &MeshObject::materialRef>;
  ReferenceWidget* referenceWidget = new ReferenceWidget(
                                                object,
                                                &MeshObject::materialRefChanged,
                                                undoStack,
                                                materialsGroup);
  _ui->materialLayout->addWidget(referenceWidget);

  adjustSize();

  connect(&object,
          &MeshObject::materialRefChanged,
          this,
          &MeshWidget::_updateMaterialWidget,
          Qt::DirectConnection);
  _updateMaterialWidget();
}

void MeshWidget::_updateMaterialWidget() noexcept
{
  if (_materialWidget != nullptr)
  {
    delete _materialWidget;
    _materialWidget = nullptr;
  }

  MaterialObject* material = _object.materialRef().get();
  if(material != nullptr)
  {
    _materialWidget = new MaterialWidget(*material, _undoStack);
    _ui->materialLayout->addWidget(_materialWidget);
  }
}

MeshWidget::~MeshWidget() noexcept = default;
