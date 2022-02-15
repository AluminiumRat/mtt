#include <SceneTab/DirectLightWidget.h>

#include <GeneratedFiles/ui_DirectLightWidget.h>

DirectLightWidget::DirectLightWidget( mtt::DirectLightObject& object,
                                      mtt::UndoStack& undoStack) :
  _ui(new Ui::DirectLightWidget)
{
  _ui->setupUi(this);

  _shadowsConnection.emplace( *_ui->shadowsCheckbox,
                              object,
                              &mtt::DirectLightObject::shadowsEnabled,
                              &mtt::DirectLightObject::setShadowsEnabled,
                              &mtt::DirectLightObject::shadowsEnabledChanged,
                              undoStack);

  _radiusConnection.emplace(*_ui->radiusSpin,
                            object,
                            &mtt::DirectLightObject::radius,
                            &mtt::DirectLightObject::setRadius,
                            &mtt::DirectLightObject::radiusChanged,
                            undoStack);

  _blurConnection.emplace(*_ui->blurSpin,
                          object,
                          &mtt::DirectLightObject::blurSize,
                          &mtt::DirectLightObject::setBlurSize,
                          &mtt::DirectLightObject::blurSizeChanged,
                          undoStack);

  _mapSizeConnection.emplace( *_ui->mapSizeSpin,
                              object,
                              &mtt::DirectLightObject::shadowmapSize,
                              &mtt::DirectLightObject::setShadowmapSize,
                              &mtt::DirectLightObject::shadowmapSizeChanged,
                              undoStack);

  _cascadeSizeConnection.emplace( *_ui->cascadeSizeSpin,
                                  object,
                                  &mtt::DirectLightObject::cascadeSize,
                                  &mtt::DirectLightObject::setCascadeSize,
                                  &mtt::DirectLightObject::cascadeSizeChanged,
                                  undoStack);
}

DirectLightWidget::~DirectLightWidget() noexcept = default;