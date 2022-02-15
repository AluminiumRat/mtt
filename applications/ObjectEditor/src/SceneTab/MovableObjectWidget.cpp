#include <mtt/application/Widgets/PropertiesWidgets/ColorPropertyWidget.h>
#include <mtt/application/Widgets/PropertiesWidgets/RotationPropertyWidget.h>
#include <mtt/application/Widgets/PropertiesWidgets/Vec3PropertyWidget.h>

#include <SceneTab/MovableObjectWidget.h>

#include <GeneratedFiles/ui_MovableObjectWidget.h>

MovableObjectWidget::MovableObjectWidget( mtt::MovableObject& object,
                                          mtt::UndoStack& undoStack) :
  _ui(new Ui::MovableObjectWidget)
{
  _ui->setupUi(this);

  mtt::Vec3PropertyWidget<mtt::MovableObject>* positionWidget =
                  new mtt::Vec3PropertyWidget<mtt::MovableObject>(
                                          object,
                                          &mtt::MovableObject::position,
                                          &mtt::MovableObject::setPosition,
                                          &mtt::MovableObject::positionChanged,
                                          undoStack);
  _ui->positionLayout->addWidget(positionWidget, 3);

  mtt::RotationPropertyWidget<mtt::MovableObject>* rotationWidget =
              new mtt::RotationPropertyWidget<mtt::MovableObject>(
                                          object,
                                          &mtt::MovableObject::rotation,
                                          &mtt::MovableObject::setRotation,
                                          &mtt::MovableObject::rotationChanged,
                                          undoStack);
  _ui->rotationLayout->addWidget(rotationWidget, 3);

  adjustSize();
}

MovableObjectWidget::~MovableObjectWidget() noexcept = default;;
