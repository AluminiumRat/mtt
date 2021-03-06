#include <mtt/application/Widgets/PropertiesWidgets/ColorPropertyWidget.h>

#include <PropertiesWidget/EmitterWidget.h>

#include <GeneratedFiles/ui_EmitterWidget.h>

EmitterWidget::EmitterWidget(EmitterObject& object, mtt::UndoStack& undoStack) :
  _ui(new Ui::EmitterWidget),
  _emitter(object)
{
  _ui->setupUi(this);

  _enabledConnection.emplace( *_ui->enabledCheck,
                              object,
                              &EmitterObject::enabled,
                              &EmitterObject::setEnabled,
                              &EmitterObject::enabledChanged,
                              undoStack);

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

  _sizeRangeConnection.emplace( *_ui->minSizeSpin,
                                *_ui->maxSizeSpin,
                                object,
                                &EmitterObject::sizeRange,
                                &EmitterObject::setSizeRange,
                                &EmitterObject::sizeRangeChanged,
                                undoStack);

  _rotationConnection.emplace(*_ui->minRotationSpin,
                              *_ui->maxRotationSpin,
                              object,
                              &EmitterObject::rotationRange,
                              &EmitterObject::setRotationRange,
                              &EmitterObject::rotationRangeChanged,
                              undoStack);
  _rotationConnection->setMultiplier(360.f / 2.f / glm::pi<float>());

  _rotationSpeedConnection.emplace( *_ui->minRotationSpeedSpin,
                                    *_ui->maxRotationSpeedSpin,
                                    object,
                                    &EmitterObject::rotationSpeedRange,
                                    &EmitterObject::setRotationSpeedRange,
                                    &EmitterObject::rotationSpeedRangeChanged,
                                    undoStack);
  _rotationSpeedConnection->setMultiplier(360.f / 2.f / glm::pi<float>());

  using ColorWidget = mtt::ColorPropertyWidget<EmitterObject>;
  _ui->albedoWidgetLayout->addWidget(
                            new ColorWidget(object,
                                            &EmitterObject::firstAlbedo,
                                            &EmitterObject::setFirstAlbedo,
                                            &EmitterObject::firstAlbedoChanged,
                                            undoStack));
  _ui->albedoWidgetLayout->addWidget(
                          new ColorWidget(object,
                                          &EmitterObject::secondAlbedo,
                                          &EmitterObject::setSecondAlbedo,
                                          &EmitterObject::secondAlbedoChanged,
                                          undoStack));

  _opacityConnection.emplace( *_ui->minOpacitySpin,
                              *_ui->maxOpacitySpin,
                              object,
                              &EmitterObject::opacityRange,
                              &EmitterObject::setOpacityRange,
                              &EmitterObject::opacityRangeChanged,
                              undoStack);

  _ui->emissionColorLayout->addWidget(
                          new ColorWidget(object,
                                          &EmitterObject::emissionColor,
                                          &EmitterObject::setEmissionColor,
                                          &EmitterObject::emissionColorChanged,
                                          undoStack));

  _emissionBrigtnessConnecton.emplace(*_ui->emissionBrightnessSpin,
                                      object,
                                      &EmitterObject::emissionBrightness,
                                      &EmitterObject::setEmissionBrightness,
                                      &EmitterObject::emissionBrightnessChanged,
                                      undoStack);

  _textureConnection.emplace( *_ui->textureIndexSpin,
                              object,
                              &EmitterObject::textureIndex,
                              &EmitterObject::setTextureIndex,
                              &EmitterObject::textureIndexChanged,
                              undoStack);

  _tileConnection.emplace(*_ui->tileIndexSpin,
                          object,
                          &EmitterObject::tileIndex,
                          &EmitterObject::setTileIndex,
                          &EmitterObject::tileIndexChanged,
                          undoStack);

  _lifetimeConnection.emplace(*_ui->minLifetimeSpin,
                              *_ui->maxLifetimeSpin,
                              object,
                              &EmitterObject::lifetimeRange,
                              &EmitterObject::setLifetimeRange,
                              &EmitterObject::lifetimeRangeChanged,
                              undoStack);

  _massConnection.emplace(*_ui->minMassSpin,
                          *_ui->maxMassSpin,
                          object,
                          &EmitterObject::massRange,
                          &EmitterObject::setMassRange,
                          &EmitterObject::massRangeChanged,
                          undoStack);

  _frictionFactorConnection.emplace(*_ui->minFrictionFactorSpin,
                                    *_ui->maxFrictionFactorSpin,
                                    object,
                                    &EmitterObject::frictionFactorRange,
                                    &EmitterObject::setFrictionFactorRange,
                                    &EmitterObject::frictionFactorRangeChanged,
                                    undoStack);

  adjustSize();
}

EmitterWidget::~EmitterWidget() noexcept = default;
