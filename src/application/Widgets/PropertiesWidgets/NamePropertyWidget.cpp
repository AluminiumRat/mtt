#include <stdexcept>

#include <mtt/application/Widgets/PropertiesWidgets/NamePropertyWidget.h>

#include <mtt/application/EditCommands/SetPropertyCommand.h>
#include <mtt/application/EditCommands/UndoStack.h>
#include <mtt/application/Scene/Object.h>
#include <mtt/utilities/Log.h>
#include <mtt/utilities/ScopedSetter.h>

#include <GeneratedFiles/ui_NamePropertyWidget.h>

using namespace mtt;

NamePropertyWidget::NamePropertyWidget(Object& object, UndoStack& undoStack) :
  _ui(new Ui::NamePropertyWidget),
  _object(object),
  _undoStack(undoStack),
  _skipUpdate(false)
{
  _ui->setupUi(this);

  connect(_ui->nameEdit,
          &QLineEdit::editingFinished,
          this,
          &NamePropertyWidget::_updateObject,
          Qt::DirectConnection);

  connect(&_object,
          &Object::nameChanged,
          this,
          &NamePropertyWidget::_updateWidget,
          Qt::DirectConnection);

  _updateWidget();

  _ui->nameEdit->setEnabled(_object.canBeRenamed());
}

NamePropertyWidget::~NamePropertyWidget() noexcept = default;

void NamePropertyWidget::_updateObject() noexcept
{
  if(_skipUpdate) return;
  ScopedTrueSetter skipper(_skipUpdate);

  try
  {
    QString newValue = _ui->nameEdit->text();
    if (_object.name() == newValue) return;

    _undoStack.addAndMake(makeSetPropertyCommand( _object,
                                                  &Object::name,
                                                  &Object::tryRename,
                                                  newValue));
  }
  catch (std::exception error)
  {
    Log() << error.what();
    Log() << "NamePropertyWidget::_updateObject: unable to update property.";
  }
  catch(...)
  {
    Log() << "NamePropertyWidget::_updateObject: unable to update property.";
  }
}

void NamePropertyWidget::_updateWidget() noexcept
{
  if (_skipUpdate) return;
  ScopedTrueSetter skipper(_skipUpdate);

  try
  {
    _ui->nameEdit->setText(_object.name());
  }
  catch (std::exception error)
  {
    Log() << error.what();
    Log() << "NamePropertyWidget::_updateWidget: unable to update widget.";
  }
  catch (...)
  {
    Log() << ("StringPropertyWidget::_updateWidget: unable to update widget.");
  }
}
