#include <PropertiesWidget/HeaterWidget.h>

#include <GeneratedFiles/ui_HeaterWidget.h>

HeaterWidget::HeaterWidget(HeaterObject& object, mtt::UndoStack& undoStack) :
  _ui(new Ui::HeaterWidget)
{
  _ui->setupUi(this);

  _sizeConnection.emplace(*_ui->sizeSpin,
                          object,
                          &HeaterObject::size,
                          &HeaterObject::setSize,
                          &HeaterObject::sizeChanged,
                          undoStack);

  _powerConnection.emplace( *_ui->powerSpin,
                            object,
                            &HeaterObject::power,
                            &HeaterObject::setPower,
                            &HeaterObject::powerChanged,
                            undoStack);
}

HeaterWidget::~HeaterWidget() noexcept = default;
