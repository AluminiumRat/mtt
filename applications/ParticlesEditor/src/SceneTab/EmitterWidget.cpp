#include <SceneTab/EmitterWidget.h>

#include <GeneratedFiles/ui_EmitterWidget.h>

EmitterWidget::EmitterWidget(EmitterObject& object, mtt::UndoStack& undoStack) :
  _ui(new Ui::EmitterWidget),
  _emitter(object)
{
  _ui->setupUi(this);

  _intensityConnection.emplace( *_ui->intensitySpin,
                                object,
                                &EmitterObject::intensity,
                                &EmitterObject::setIntensity,
                                &EmitterObject::intensityChanged,
                                undoStack);

  _sizeConnection.emplace(*_ui->sizeSpin,
                          object,
                          &EmitterObject::size,
                          &EmitterObject::setSize,
                          &EmitterObject::sizeChanged,
                          undoStack);

  _shapeConnection.emplace( *_ui->shapeCombo,
                            object,
                            &EmitterObject::shape,
                            &EmitterObject::setShape,
                            &EmitterObject::shapeChanged,
                            EmitterObject::shapeNames,
                            undoStack);

  _distributionConnection.emplace(*_ui->distributionCombo,
                                  object,
                                  &EmitterObject::distribution,
                                  &EmitterObject::setDistribution,
                                  &EmitterObject::distributionChanged,
                                  EmitterObject::distributionNames,
                                  undoStack);

  adjustSize();
}

EmitterWidget::~EmitterWidget() noexcept = default;
