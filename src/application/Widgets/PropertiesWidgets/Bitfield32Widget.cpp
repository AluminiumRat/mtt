#include <mtt/application/Widgets/PropertiesWidgets/Bitfield32Widget.h>
#include <mtt/utilities/ScopedSetter.h>

#include <GeneratedFiles/ui_Bitfield32Widget.h>

using namespace mtt;

Bitfield32Widget::Bitfield32Widget() :
  _ui(new Ui::Bitfield32Widget),
  _value(0),
  _skipUpdate(false)
{
  _ui->setupUi(this);

  _addCheckbox(*_ui->checkBox0, 0);
  _addCheckbox(*_ui->checkBox1, 1);
  _addCheckbox(*_ui->checkBox2, 2);
  _addCheckbox(*_ui->checkBox3, 3);
  _addCheckbox(*_ui->checkBox4, 4);
  _addCheckbox(*_ui->checkBox5, 5);
  _addCheckbox(*_ui->checkBox6, 6);
  _addCheckbox(*_ui->checkBox7, 7);
  _addCheckbox(*_ui->checkBox8, 8);
  _addCheckbox(*_ui->checkBox9, 9);
  _addCheckbox(*_ui->checkBox10, 10);
  _addCheckbox(*_ui->checkBox11, 11);
  _addCheckbox(*_ui->checkBox12, 12);
  _addCheckbox(*_ui->checkBox13, 13);
  _addCheckbox(*_ui->checkBox14, 14);
  _addCheckbox(*_ui->checkBox15, 15);
  _addCheckbox(*_ui->checkBox16, 16);
  _addCheckbox(*_ui->checkBox17, 17);
  _addCheckbox(*_ui->checkBox18, 18);
  _addCheckbox(*_ui->checkBox19, 19);
  _addCheckbox(*_ui->checkBox20, 20);
  _addCheckbox(*_ui->checkBox21, 21);
  _addCheckbox(*_ui->checkBox22, 22);
  _addCheckbox(*_ui->checkBox23, 23);
  _addCheckbox(*_ui->checkBox24, 24);
  _addCheckbox(*_ui->checkBox25, 25);
  _addCheckbox(*_ui->checkBox26, 26);
  _addCheckbox(*_ui->checkBox27, 27);
  _addCheckbox(*_ui->checkBox28, 28);
  _addCheckbox(*_ui->checkBox29, 29);
  _addCheckbox(*_ui->checkBox30, 30);
  _addCheckbox(*_ui->checkBox31, 31);

  _updateValue();
}

void Bitfield32Widget::_addCheckbox(QCheckBox& checkbox, size_t index)
{
  _checkboxes[index] = &checkbox;

  connect(&checkbox,
          &QCheckBox::stateChanged,
          this,
          &Bitfield32Widget::_updateValue,
          Qt::DirectConnection);
}

Bitfield32Widget::~Bitfield32Widget() noexcept = default;

void Bitfield32Widget::_updateValue() noexcept
{
  if (_skipUpdate) return;
  ScopedTrueSetter skipper(_skipUpdate);

  uint32_t newValue = 0;
  uint32_t cursor = 1;
  for (size_t iCheckbox = 0; iCheckbox < 32; iCheckbox++)
  {
    if(_checkboxes[iCheckbox]->checkState() == Qt::Checked) newValue += cursor;
    cursor *= 2;
  }

  setValue(newValue);
}

void Bitfield32Widget::setValue(uint32_t newValue) noexcept
{
  if(_value == newValue) return;
  _value = newValue;
  _updateWidgets();
  emit valueChanged(newValue);
}

void Bitfield32Widget::_updateWidgets() noexcept
{
  if (_skipUpdate) return;
  ScopedTrueSetter skipper(_skipUpdate);

  uint32_t cursor = 1;
  for (size_t iCheckbox = 0; iCheckbox < 32; iCheckbox++)
  {
    _checkboxes[iCheckbox]->setChecked((_value & cursor));
    cursor *= 2;
  }
}
