#include <mtt/application/Widgets/PropertiesWidgets/ColorPropertyWidget.h>
#include <mtt/application/Widgets/PropertiesWidgets/RotationPropertyWidget.h>
#include <mtt/application/Widgets/PropertiesWidgets/Vec3PropertyWidget.h>

#include <SceneTab/MovableObjectWidget.h>

#include <GeneratedFiles/ui_MovableObjectWidget.h>

MovableObjectWidget::MovableObjectWidget( MovableObject& object,
                                          mtt::UndoStack& undoStack) :
  _ui(new Ui::MovableObjectWidget)
{
  _ui->setupUi(this);

  mtt::Vec3PropertyWidget<MovableObject>* positionWidget =
                  new mtt::Vec3PropertyWidget<MovableObject>(
                                                object,
                                                &MovableObject::position,
                                                &MovableObject::setPosition,
                                                &MovableObject::positionChanged,
                                                undoStack);
  _ui->positionLayout->addWidget(positionWidget, 3);

  mtt::RotationPropertyWidget<MovableObject>* rotationWidget =
              new mtt::RotationPropertyWidget<MovableObject>(
                                                object,
                                                &MovableObject::rotation,
                                                &MovableObject::setRotation,
                                                &MovableObject::rotationChanged,
                                                undoStack);
  _ui->rotationLayout->addWidget(rotationWidget, 3);

  adjustSize();
}

MovableObjectWidget::~MovableObjectWidget() noexcept = default;;
