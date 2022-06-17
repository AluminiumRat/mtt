#include <mtt/application/Widgets/PropertiesWidgets/FilenamePropertyWidget.h>
#include <mtt/editorLib/PropertiesWidget/ShadowsWidget.h>
#include <mtt/editorLib/PropertiesWidget/SpotLightWidget.h>

#include <GeneratedFiles/ui_SpotLightWidget.h>

using namespace mtt;

SpotLightWidget::SpotLightWidget( SpotLightObject& object,
                                  UndoStack& undoStack) :
  _ui(new Ui::SpotLightWidget)
{
  _ui->setupUi(this);

  _angleConnection.emplace( *_ui->angleSpin,
                            object,
                            &SpotLightObject::angle,
                            &SpotLightObject::setAngle,
                            &SpotLightObject::angleChanged,
                            undoStack);
  _angleConnection->setMultiplier(180.f / glm::pi<float>());

  using FilterWidget = FilenamePropertyWidget<SpotLightObject>;
  FilterWidget* filterWidget =
            new FilterWidget( object,
                              &SpotLightObject::filterImage,
                              &SpotLightObject::setFilterImage,
                              &SpotLightObject::filterImageChanged,
                              tr("picture files(*.png *.jpg *.jpeg *.bmp)"),
                              undoStack);
  _ui->filterLayout->addWidget(filterWidget, 3);

  _ui->shadowsLayout->addWidget(new ShadowsWidget(object,
                                                  180.f / glm::pi<float>(),
                                                  20.f,
                                                  undoStack));
}

SpotLightWidget::~SpotLightWidget() noexcept = default;