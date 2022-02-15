#include <mtt/application/Widgets/PropertiesWidgets/ColorPropertyWidget.h>

#include <SceneTab/BackgroundWidget.h>
#include <SceneTab/CubemapWidget.h>

#include <GeneratedFiles/ui_BackgroundWidget.h>

BackgroundWidget::BackgroundWidget( mtt::BackgroundObject& object,
                                    mtt::UndoStack& undoStack) :
  _ui(new Ui::BackgroundWidget)
{
  _ui->setupUi(this);

  _lightConnection.emplace( *_ui->lightCheckbox,
                            object,
                            &mtt::BackgroundObject::lightEnabled,
                            &mtt::BackgroundObject::setLightEnabled,
                            &mtt::BackgroundObject::lightEnabledChanged,
                            undoStack);

  mtt::ColorPropertyWidget<mtt::BackgroundObject>* colorWidget =
                  new mtt::ColorPropertyWidget<mtt::BackgroundObject>(
                                          object,
                                          &mtt::BackgroundObject::color,
                                          &mtt::BackgroundObject::setColor,
                                          &mtt::BackgroundObject::colorChanged,
                                          undoStack);
  _ui->colorLayout->addWidget(colorWidget, 3);

  _luminanceConnection.emplace( *_ui->luminanceSpin,
                                object,
                                &mtt::BackgroundObject::luminance,
                                &mtt::BackgroundObject::setLuminance,
                                &mtt::BackgroundObject::luminanceChanged,
                                undoStack);

  _startConnection.emplace(
                        *_ui->startSpin,
                        object,
                        &mtt::BackgroundObject::dissolutionStartDistance,
                        &mtt::BackgroundObject::setDissolutionStartDistance,
                        &mtt::BackgroundObject::dissolutionStartDistanceChanged,
                        undoStack);

  _lengthConnection.emplace( *_ui->lengthSpin,
                            object,
                            &mtt::BackgroundObject::dissolutionLength,
                            &mtt::BackgroundObject::setDissolutionLength,
                            &mtt::BackgroundObject::dissolutionLengthChanged,
                            undoStack);

  CubemapWidget* cubemapWidget = new CubemapWidget( object.cubemap(),
                                                    undoStack);
  _ui->cubemapLayout->addWidget(cubemapWidget);
  adjustSize();
}

BackgroundWidget::~BackgroundWidget() noexcept = default;;
