#include <mtt/editorLib/PropertiesWidget/DirectLightWidget.h>
#include <mtt/editorLib/PropertiesWidget/ShadowsWidget.h>

#include <GeneratedFiles/ui_DirectLightWidget.h>

using namespace mtt;

DirectLightWidget::DirectLightWidget( DirectLightObject& object,
                                      UndoStack& undoStack) :
  _ui(new Ui::DirectLightWidget)
{
  _ui->setupUi(this);

  _radiusConnection.emplace(*_ui->radiusSpin,
                            object,
                            &DirectLightObject::radius,
                            &DirectLightObject::setRadius,
                            &DirectLightObject::radiusChanged,
                            undoStack);

  _cascadeSizeConnection.emplace( *_ui->cascadeSizeSpin,
                                  object,
                                  &DirectLightObject::cascadeSize,
                                  &DirectLightObject::setCascadeSize,
                                  &DirectLightObject::cascadeSizeChanged,
                                  undoStack);

  _ui->shadowsLayout->addWidget(new ShadowsWidget(object,
                                                  1,
                                                  10.f,
                                                  undoStack));
}

DirectLightWidget::~DirectLightWidget() noexcept = default;