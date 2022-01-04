#include <mtt/Application/Widgets/PropertiesWidgets/ReferencePropertyWidget.h>

#include <SceneTab/MaterialWidget.h>
#include <SceneTab/MeshWidget.h>
#include <SceneTab/VertexDataWidget.h>

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

  using ReferenceWidget = mtt::ReferencePropertyWidget< MeshObject,
                                                        MaterialObject>;
  ReferenceWidget* referenceWidget = new ReferenceWidget(
                                          object,
                                          &MeshObject::material,
                                          &MeshObject::setMaterial,
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

  if(_object.material() != nullptr)
  {
    _materialWidget = new MaterialWidget(*_object.material(), _undoStack);
    _ui->materialLayout->addWidget(_materialWidget);
  }
}

MeshWidget::~MeshWidget() noexcept = default;
