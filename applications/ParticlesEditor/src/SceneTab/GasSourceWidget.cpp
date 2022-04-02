#include <SceneTab/GasSourceWidget.h>

#include <GeneratedFiles/ui_GasSourceWidget.h>

GasSourceWidget::GasSourceWidget(GasSource& object, mtt::UndoStack& undoStack) :
  _ui(new Ui::GasSourceWidget)
{
  _ui->setupUi(this);

  _sizeConnection.emplace(*_ui->sizeSpin,
                          object,
                          &GasSource::size,
                          &GasSource::setSize,
                          &GasSource::sizeChanged,
                          undoStack);

  _flowRateConnection.emplace(*_ui->flowRateSpin,
                              object,
                              &GasSource::flowRate,
                              &GasSource::setFlowRate,
                              &GasSource::flowRateChanged,
                              undoStack);

  _temperatureConnection.emplace( *_ui->temperatureSpin,
                                  object,
                                  &GasSource::temperature,
                                  &GasSource::setTemperature,
                                  &GasSource::temperatureChanged,
                                  undoStack);
}

GasSourceWidget::~GasSourceWidget() noexcept = default;
