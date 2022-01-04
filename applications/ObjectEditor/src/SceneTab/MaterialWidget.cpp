#include <mtt/Application/Widgets/PropertiesWidgets/ColorPropertyWidget.h>
#include <mtt/Application/Widgets/PropertiesWidgets/FloatPropertyLinearSlider.h>
#include <mtt/Application/Widgets/PropertiesWidgets/FloatPropertyProgressiveSlider.h>
#include <mtt/Application/Widgets/PropertiesWidgets/FloatPropertySpin.h>

#include <SceneTab/FilenamePropertyWidget.h>
#include <SceneTab/MaterialWidget.h>

#include <GeneratedFiles/ui_MaterialWidget.h>

MaterialWidget::MaterialWidget( MaterialObject& object,
                                mtt::UndoStack& undoStack) :
  _ui(new Ui::MaterialWidget)
{
  _ui->setupUi(this);

  _useAplphaConnection.emplace(
                              *_ui->useAlphaBox,
                              object,
                              &MaterialObject::useAlphaFromAlbedoTexture,
                              &MaterialObject::setUseAlphaFromAlbedoTexture,
                              &MaterialObject::useAlphaFromAlbedoTextureChanged,
                              undoStack);

  using ColorWidget = mtt::ColorPropertyWidget<MaterialObject>;
  ColorWidget* albedoColorWidget = new ColorWidget( 
                                                object,
                                                &MaterialObject::albedo,
                                                &MaterialObject::setAlbedo,
                                                &MaterialObject::albedoChanged,
                                                undoStack);
  _ui->albedoColorLayout->addWidget(albedoColorWidget, 3);

  using TextureWidget = FilenamePropertyWidget<MaterialObject>;
  TextureWidget* albedoTextureWidget =
                new TextureWidget(object,
                                  &MaterialObject::albedoTexture,
                                  &MaterialObject::setAlbedoTexture,
                                  &MaterialObject::albedoTextureChanged,
                                  tr("picture files(*.png *.jpg *.jpeg *.bmp)"),
                                  undoStack);
  _ui->albedoTextureLayout->addWidget(albedoTextureWidget, 3);

  using FloatSlider = mtt::FloatPropertyLinearSlider<MaterialObject>;
  FloatSlider* opaqueWidget = new FloatSlider(
                                          object,
                                          &MaterialObject::opaqueFactor,
                                          &MaterialObject::setOpaqueFactor,
                                          &MaterialObject::opaqueFactorChanged,
                                          undoStack);
  _ui->opaqueFactorLayout->addWidget(opaqueWidget, 3);

  TextureWidget* opaqueTextureWidget =
                new TextureWidget(object,
                                  &MaterialObject::opaqueTexture,
                                  &MaterialObject::setOpaqueTexture,
                                  &MaterialObject::opaqueTextureChanged,
                                  tr("picture files(*.png *.jpg *.jpeg *.bmp)"),
                                  undoStack);
  _ui->opaqueTextureLayout->addWidget(opaqueTextureWidget, 3);

  using FloatSlider = mtt::FloatPropertyLinearSlider<MaterialObject>;
  FloatSlider* roughnessWidget = new FloatSlider(
                                              object,
                                              &MaterialObject::roughness,
                                              &MaterialObject::setRoughness,
                                              &MaterialObject::roughnessChanged,
                                              undoStack);
  _ui->roughnessLayout->addWidget(roughnessWidget, 3);

  FloatSlider* specularStrengthWidget = new FloatSlider(
                                      object,
                                      &MaterialObject::specularStrength,
                                      &MaterialObject::setSpecularStrength,
                                      &MaterialObject::specularStrengthChanged,
                                      undoStack);
  _ui->strengthLayout->addWidget(specularStrengthWidget, 3);

  TextureWidget* specularTextureWidget =
                new TextureWidget(object,
                                  &MaterialObject::specularTexture,
                                  &MaterialObject::setSpecularTexture,
                                  &MaterialObject::specularTextureChanged,
                                  tr("picture files(*.png *.jpg *.jpeg *.bmp)"),
                                  undoStack);
  _ui->specularTextureLayout->addWidget(specularTextureWidget, 3);

  FloatSlider* metallicWidget = new FloatSlider(
                                              object,
                                              &MaterialObject::metallic,
                                              &MaterialObject::setMetallic,
                                              &MaterialObject::metallicChanged,
                                              undoStack);
  _ui->metallicLayout->addWidget(metallicWidget, 3);

  TextureWidget* normalTextureWidget =
                new TextureWidget(object,
                                  &MaterialObject::normalTexture,
                                  &MaterialObject::setNormalTexture,
                                  &MaterialObject::normalTextureChanged,
                                  tr("picture files(*.png *.jpg *.jpeg *.bmp)"),
                                  undoStack);
  _ui->normalTextureLayout->addWidget(normalTextureWidget, 3);

  using ProgressiveSlider =
                          mtt::FloatPropertyProgressiveSlider<MaterialObject>;
  ProgressiveSlider* emissionFactorWidget = new ProgressiveSlider(
                                        object,
                                        &MaterialObject::emissionFactor,
                                        &MaterialObject::setEmissionFactor,
                                        &MaterialObject::emissionFactorChanged,
                                        undoStack);
  _ui->emissionFactorWidget->layout()->addWidget(emissionFactorWidget);
  emissionFactorWidget->setRange(0, 100);

  using FloatSpin = mtt::FloatPropertySpin<MaterialObject>;
  FloatSpin* emissionFactorSpin = new FloatSpin(
                                      object,
                                      &MaterialObject::emissionFactor,
                                      &MaterialObject::setEmissionFactor,
                                      &MaterialObject::emissionFactorChanged,
                                      undoStack);
  _ui->emissionFactorWidget->layout()->addWidget(emissionFactorSpin);

  ColorWidget* emissionColorWidget = new ColorWidget(
                                          object,
                                          &MaterialObject::emissionColor,
                                          &MaterialObject::setEmissionColor,
                                          &MaterialObject::emissionColorChanged,
                                          undoStack);
  _ui->emissionColorLayout->addWidget(emissionColorWidget, 3);

  TextureWidget* emissionTextureWidget =
                new TextureWidget(object,
                                  &MaterialObject::emissionTexture,
                                  &MaterialObject::setEmissionTexture,
                                  &MaterialObject::emissionTextureChanged,
                                  tr("picture files(*.png *.jpg *.jpeg *.bmp)"),
                                  undoStack);
  _ui->emissionTextureLayout->addWidget(emissionTextureWidget, 3);

  FloatSlider* reflectionFactorWidget = new FloatSlider(
                                      object,
                                      &MaterialObject::reflectionFactor,
                                      &MaterialObject::setReflectionFactor,
                                      &MaterialObject::reflectionFactorChanged,
                                      undoStack);
  _ui->reflectionFactorLayout->addWidget(reflectionFactorWidget, 3);

  TextureWidget* reflectionTextureWidget =
                new TextureWidget(object,
                                  &MaterialObject::reflectionTexture,
                                  &MaterialObject::setReflectionTexture,
                                  &MaterialObject::reflectionTextureChanged,
                                  tr("picture files(*.png *.jpg *.jpeg *.bmp)"),
                                  undoStack);
  _ui->reflectionTextureLayout->addWidget(reflectionTextureWidget, 3);

  adjustSize();
}

MaterialWidget::~MaterialWidget() noexcept = default;
