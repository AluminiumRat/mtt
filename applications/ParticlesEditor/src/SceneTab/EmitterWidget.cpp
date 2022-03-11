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

  _angleConnection.emplace( *_ui->directionAngleSpin,
                            object,
                            &EmitterObject::directionAngle,
                            &EmitterObject::setDirectionAngle,
                            &EmitterObject::directionAngleChanged,
                            undoStack);
  _angleConnection->setMultiplier(360.f / 2.f / glm::pi<float>());

  _speedConnection.emplace( *_ui->minSpeedSpin,
                            *_ui->maxSpeedSpin,
                            object,
                            &EmitterObject::speedRange,
                            &EmitterObject::setSpeedRange,
                            &EmitterObject::speedRangeChanged,
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

  connect(_ui->emitButton,
          &QPushButton::pressed,
          this,
          &EmitterWidget::emitParticles,
          Qt::DirectConnection);

  connect(_ui->stepButton,
          &QPushButton::pressed,
          this,
          &EmitterWidget::step,
          Qt::DirectConnection);

  adjustSize();
}

EmitterWidget::~EmitterWidget() noexcept = default;

void EmitterWidget::emitParticles() noexcept
{
  _emitter.emitParticles(100);
}

void EmitterWidget::step() noexcept
{
  _emitter.simulationStep(
                      ParticleField::TimeType(0),
                      std::chrono::duration_cast<ParticleField::TimeType>(
                                              std::chrono::milliseconds(100)));
}
