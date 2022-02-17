#include <mtt/application/Widgets/PropertiesWidgets/ColorPropertyWidget.h>
#include <mtt/application/Widgets/PropertiesWidgets/RotationPropertyWidget.h>
#include <mtt/application/Widgets/PropertiesWidgets/Vec3PropertyWidget.h>
#include <mtt/editorLib/SceneTab/RotatableObjectWidget.h>

#include <GeneratedFiles/ui_RotatableObjectWidget.h>

using namespace mtt;

RotatableObjectWidget::RotatableObjectWidget( RotatableObject& object,
                                              UndoStack& undoStack) :
  _ui(new Ui::RotatableObjectWidget)
{
  _ui->setupUi(this);

  RotationPropertyWidget<RotatableObject>* rotationWidget =
                    new RotationPropertyWidget<RotatableObject>(
                                              object,
                                              &RotatableObject::rotation,
                                              &RotatableObject::setRotation,
                                              &RotatableObject::rotationChanged,
                                              undoStack);
  _ui->rotationLayout->addWidget(rotationWidget, 3);

  adjustSize();
}

RotatableObjectWidget::~RotatableObjectWidget() noexcept = default;;
