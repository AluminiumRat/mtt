#include <mtt/application/Widgets/PropertiesWidgets/ColorPropertyWidget.h>
#include <mtt/editorLib/PropertiesWidget/BackgroundWidget.h>
#include <mtt/editorLib/PropertiesWidget/CubemapWidget.h>

#include <GeneratedFiles/ui_BackgroundWidget.h>

using namespace mtt;

BackgroundWidget::BackgroundWidget( BackgroundObject& object,
                                    UndoStack& undoStack) :
  _ui(new Ui::BackgroundWidget)
{
  _ui->setupUi(this);

  _lightConnection.emplace( *_ui->lightCheckbox,
                            object,
                            &BackgroundObject::lightEnabled,
                            &BackgroundObject::setLightEnabled,
                            &BackgroundObject::lightEnabledChanged,
                            undoStack);

  ColorPropertyWidget<BackgroundObject>* colorWidget =
                      new ColorPropertyWidget<BackgroundObject>(
                                                object,
                                                &BackgroundObject::color,
                                                &BackgroundObject::setColor,
                                                &BackgroundObject::colorChanged,
                                                undoStack);
  _ui->colorLayout->addWidget(colorWidget, 3);

  _luminanceConnection.emplace( *_ui->luminanceSpin,
                                object,
                                &BackgroundObject::luminance,
                                &BackgroundObject::setLuminance,
                                &BackgroundObject::luminanceChanged,
                                undoStack);

  _startConnection.emplace( *_ui->startSpin,
                            object,
                            &BackgroundObject::dissolutionStartDistance,
                            &BackgroundObject::setDissolutionStartDistance,
                            &BackgroundObject::dissolutionStartDistanceChanged,
                            undoStack);

  _lengthConnection.emplace( *_ui->lengthSpin,
                            object,
                            &BackgroundObject::dissolutionLength,
                            &BackgroundObject::setDissolutionLength,
                            &BackgroundObject::dissolutionLengthChanged,
                            undoStack);

  CubemapWidget* cubemapWidget = new CubemapWidget( object.cubemap(),
                                                    undoStack);
  _ui->cubemapLayout->addWidget(cubemapWidget);
  adjustSize();
}

BackgroundWidget::~BackgroundWidget() noexcept = default;;
