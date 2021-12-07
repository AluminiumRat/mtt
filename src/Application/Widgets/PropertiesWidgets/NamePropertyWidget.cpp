#include <stdexcept>

#include <mtt/Application/Widgets/PropertiesWidgets/NamePropertyWidget.h>

#include <mtt/Application/EditCommands/SetPropertyCommand.h>
#include <mtt/Application/EditCommands/UndoStack.h>
#include <mtt/Application/Scene/Object.h>
#include <mtt/Utilities/Log.h>
#include <mtt/Utilities/ScopedSetter.h>

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

    using Command = SetPropertyCommand< Object,
                                        QString,
                                        void (Object::*)(const QString&)>;
    std::unique_ptr<Command> command(new Command( _object,
                                                  &Object::tryRename,
                                                  _object.name(),
                                                  newValue));
    _undoStack.addAndMake(std::move(command));
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
