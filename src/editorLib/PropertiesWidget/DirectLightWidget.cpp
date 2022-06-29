#include <mtt/editorLib/PropertiesWidget/DirectLightWidget.h>
#include <mtt/editorLib/PropertiesWidget/ShadowsWidget.h>

#include <GeneratedFiles/ui_DirectLightWidget.h>

using namespace mtt;

DirectLightWidget::DirectLightWidget( DirectLightObject& object,
                                      UndoStack& undoStack) :
  _ui(new Ui::DirectLightWidget)
{
  _ui->setupUi(this);

  _shadowDistanceConnection.emplace(*_ui->shadowDistanceSpin,
                                    object,
                                    &DirectLightObject::shadowDistance,
                                    &DirectLightObject::setShadowDistance,
                                    &DirectLightObject::shadowDistanceChanged,
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