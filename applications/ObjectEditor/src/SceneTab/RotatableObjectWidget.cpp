#include <mtt/application/Widgets/PropertiesWidgets/ColorPropertyWidget.h>
#include <mtt/application/Widgets/PropertiesWidgets/RotationPropertyWidget.h>
#include <mtt/application/Widgets/PropertiesWidgets/Vec3PropertyWidget.h>

#include <SceneTab/RotatableObjectWidget.h>

#include <GeneratedFiles/ui_RotatableObjectWidget.h>

RotatableObjectWidget::RotatableObjectWidget( mtt::RotatableObject& object,
                                              mtt::UndoStack& undoStack) :
  _ui(new Ui::RotatableObjectWidget)
{
  _ui->setupUi(this);

  mtt::RotationPropertyWidget<mtt::RotatableObject>* rotationWidget =
        new mtt::RotationPropertyWidget<mtt::RotatableObject>(
                                        object,
                                        &mtt::RotatableObject::rotation,
                                        &mtt::RotatableObject::setRotation,
                                        &mtt::RotatableObject::rotationChanged,
                                        undoStack);
  _ui->rotationLayout->addWidget(rotationWidget, 3);

  adjustSize();
}

RotatableObjectWidget::~RotatableObjectWidget() noexcept = default;;
