#include <mtt/Application/Widgets/PropertiesWidgets/Vec3Widget.h>

#include <GeneratedFiles/ui_Vec3PropertyWidget.h>

using namespace mtt;

Vec3Widget::Vec3Widget() :
  _ui(new Ui::Vec3PropertyWidget()),
  _xSpin(nullptr),
  _ySpin(nullptr),
  _zSpin(nullptr)
{
  _ui->setupUi(this);
  _xSpin = _ui->xSpin;
  _ySpin = _ui->ySpin;
  _zSpin = _ui->zSpin;
}

Vec3Widget::~Vec3Widget() noexcept
{
  delete _ui;
}
