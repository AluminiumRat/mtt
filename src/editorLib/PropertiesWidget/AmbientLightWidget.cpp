#include <mtt/editorLib/PropertiesWidget/AmbientLightWidget.h>
#include <mtt/editorLib/PropertiesWidget/CubemapWidget.h>

#include <GeneratedFiles/ui_AmbientLightWidget.h>

using namespace mtt;

AmbientLightWidget::AmbientLightWidget( AmbientLightObject& object,
                                        UndoStack& undoStack) :
  _ui(new Ui::AmbientLightWidget)
{
  _ui->setupUi(this);
  _saturationConnection.emplace(*_ui->saturationSpin,
                                object,
                                &AmbientLightObject::saturationDistance,
                                &AmbientLightObject::setSaturationDistance,
                                &AmbientLightObject::saturationDistanceChanged,
                                undoStack);

  CubemapWidget* cubemapWidget = new CubemapWidget( object.ambientMap(),
                                                    undoStack);
  _ui->ambientmapLayout->addWidget(cubemapWidget);
  adjustSize();
}

AmbientLightWidget::~AmbientLightWidget() noexcept = default;;
