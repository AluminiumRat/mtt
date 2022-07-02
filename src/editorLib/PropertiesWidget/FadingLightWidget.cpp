#include <mtt/editorLib/PropertiesWidget/FadingLightWidget.h>

#include <GeneratedFiles/ui_FadingLightWidget.h>

using namespace mtt;

FadingLightWidget::FadingLightWidget(FadingLightObject& object, UndoStack& undoStack) :
  _ui(new Ui::FadingLightWidget)
{
  _ui->setupUi(this);

  _fadeTypeConnection.emplace(*_ui->fadeTypeCombo,
                              object,
                              &FadingLightObject::fadeType,
                              &FadingLightObject::setFadeType,
                              &FadingLightObject::fadeTypeChanged,
                              FadingLightObject::fadeTypeNames,
                              undoStack);
}

FadingLightWidget::~FadingLightWidget() noexcept = default;;
