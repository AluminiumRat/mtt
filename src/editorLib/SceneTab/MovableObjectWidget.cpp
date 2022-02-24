#include <mtt/application/Widgets/PropertiesWidgets/RotationPropertyWidget.h>
#include <mtt/application/Widgets/PropertiesWidgets/Vec3PropertyWidget.h>
#include <mtt/editorLib/SceneTab/MovableObjectWidget.h>

#include <GeneratedFiles/ui_MovableObjectWidget.h>

using namespace mtt;

MovableObjectWidget::MovableObjectWidget( MovableObject& object,
                                          UndoStack& undoStack) :
  _ui(new Ui::MovableObjectWidget)
{
  _ui->setupUi(this);

  Vec3PropertyWidget<MovableObject>* positionWidget =
                      new Vec3PropertyWidget<MovableObject>(
                                                object,
                                                &MovableObject::position,
                                                &MovableObject::setPosition,
                                                &MovableObject::positionChanged,
                                                undoStack);
  _ui->positionLayout->addWidget(positionWidget, 3);

  RotationPropertyWidget<MovableObject>* rotationWidget =
                        new RotationPropertyWidget<MovableObject>(
                                                object,
                                                &MovableObject::rotation,
                                                &MovableObject::setRotation,
                                                &MovableObject::rotationChanged,
                                                undoStack);
  _ui->rotationLayout->addWidget(rotationWidget, 3);

  adjustSize();
}

MovableObjectWidget::~MovableObjectWidget() noexcept = default;;
