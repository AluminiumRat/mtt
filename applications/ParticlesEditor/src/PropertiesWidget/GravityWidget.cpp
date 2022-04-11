#include <PropertiesWidget/GravityWidget.h>

#include <GeneratedFiles/ui_GravityWidget.h>

GravityWidget::GravityWidget( GravityModificator& object,
                              mtt::UndoStack& undoStack) :
  _ui(new Ui::GravityWidget)
{
  _ui->setupUi(this);

  _accelerationConnection.emplace(*_ui->accelerationSpin,
                                  object,
                                  &GravityModificator::acceleration,
                                  &GravityModificator::setAcceleration,
                                  &GravityModificator::accelerationChanged,
                                  undoStack);
}

GravityWidget::~GravityWidget() noexcept = default;
