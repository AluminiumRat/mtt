#include <SceneTab/DirectLightWidget.h>

#include <GeneratedFiles/ui_DirectLightWidget.h>

DirectLightWidget::DirectLightWidget( DirectLightObject& object,
                                      mtt::UndoStack& undoStack) :
  _ui(new Ui::DirectLightWidget)
{
  _ui->setupUi(this);

  _shadowsConnection.emplace( *_ui->shadowsCheckbox,
                              object,
                              &DirectLightObject::shadowsEnabled,
                              &DirectLightObject::setShadowsEnabled,
                              &DirectLightObject::shadowsEnabledChanged,
                              undoStack);

  _radiusConnection.emplace(*_ui->radiusSpin,
                            object,
                            &DirectLightObject::radius,
                            &DirectLightObject::setRadius,
                            &DirectLightObject::radiusChanged,
                            undoStack);

  _blurConnection.emplace(*_ui->blurSpin,
                          object,
                          &DirectLightObject::blurSize,
                          &DirectLightObject::setBlurSize,
                          &DirectLightObject::blurSizeChanged,
                          undoStack);

  _mapSizeConnection.emplace( *_ui->mapSizeSpin,
                              object,
                              &DirectLightObject::shadowmapSize,
                              &DirectLightObject::setShadowmapSize,
                              &DirectLightObject::shadowmapSizeChanged,
                              undoStack);

  _cascadeSizeConnection.emplace( *_ui->cascadeSizeSpin,
                                  object,
                                  &DirectLightObject::cascadeSize,
                                  &DirectLightObject::setCascadeSize,
                                  &DirectLightObject::cascadeSizeChanged,
                                  undoStack);
}

DirectLightWidget::~DirectLightWidget() noexcept = default;