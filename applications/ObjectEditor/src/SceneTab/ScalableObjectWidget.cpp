#include <mtt/application/Widgets/PropertiesWidgets/ColorPropertyWidget.h>
#include <mtt/application/Widgets/PropertiesWidgets/RotationPropertyWidget.h>
#include <mtt/application/Widgets/PropertiesWidgets/Vec3PropertyWidget.h>

#include <SceneTab/ScalableObjectWidget.h>

#include <GeneratedFiles/ui_ScalableObjectWidget.h>

ScalableObjectWidget::ScalableObjectWidget( ScalableObject& object,
                                            mtt::UndoStack& undoStack) :
  _ui(new Ui::ScalableObjectWidget)
{
  _ui->setupUi(this);

  mtt::Vec3PropertyWidget<ScalableObject>* positionWidget =
                  new mtt::Vec3PropertyWidget<ScalableObject>(
                                                  object,
                                                  &ScalableObject::scale,
                                                  &ScalableObject::setScale,
                                                  &ScalableObject::scaleChanged,
                                                  undoStack);
  _ui->scaleLayout->addWidget(positionWidget, 3);

  adjustSize();
}

ScalableObjectWidget::~ScalableObjectWidget() noexcept = default;;
