#include <mtt/editorLib/PropertiesWidget/ShadowsWidget.h>

#include <GeneratedFiles/ui_ShadowsWidget.h>

using namespace mtt;

ShadowsWidget::ShadowsWidget( LightWithShadowsObject& object,
                              float blurScaleFactor,
                              float maxBlurValue,
                              UndoStack& undoStack) :
  _ui(new Ui::ShadowsWidget)
{
  _ui->setupUi(this);

  _enabledConnection.emplace( *_ui->shadowsCheckbox,
                              object,
                              &LightWithShadowsObject::shadowsEnabled,
                              &LightWithShadowsObject::setShadowsEnabled,
                              &LightWithShadowsObject::shadowsEnabledChanged,
                              undoStack);

  _mapSizeConnection.emplace( *_ui->mapSizeSpin,
                              object,
                              &LightWithShadowsObject::shadowmapSize,
                              &LightWithShadowsObject::setShadowmapSize,
                              &LightWithShadowsObject::shadowmapSizeChanged,
                              undoStack);

  _ui->blurSpin->setMaximum(maxBlurValue);
  _blurConnection.emplace(*_ui->blurSpin,
                          object,
                          &LightWithShadowsObject::shadowBlur,
                          &LightWithShadowsObject::setShadowBlur,
                          &LightWithShadowsObject::shadowBlurChanged,
                          undoStack);
  _blurConnection->setMultiplier(blurScaleFactor);
}

ShadowsWidget::~ShadowsWidget() noexcept = default;