#include <glm/glm.hpp>
#include <glm/gtc/color_space.hpp>

#include <QtWidgets/QColorDialog>

#include <mtt/application/Widgets/PropertiesWidgets/ColorWidget.h>

#include <GeneratedFiles/ui_ColorPropertyWidget.h>

using namespace mtt;

ColorWidget::ColorWidget() :
  _ui(new Ui::ColorPropertyWidget),
  _color(0)
{
  _ui->setupUi(this);

  connect(_ui->changeButton,
          &QPushButton::clicked,
          this,
          &ColorWidget::_changeColor,
          Qt::DirectConnection);

  _ui->colorFrame->setAutoFillBackground(true);

  _updateColorWidget();
}

ColorWidget::~ColorWidget() noexcept
{
  delete _ui;
}

void ColorWidget::_changeColor() noexcept
{
  glm::vec3 srgbColor = glm::convertLinearToSRGB(_color);
  QColor currentColor;
  currentColor.setRgbF(srgbColor.r, srgbColor.g, srgbColor.b);

  QColor newColor = QColorDialog::getColor( currentColor,
                                            this,
                                            tr("Color"));
  if (newColor.isValid())
  {
    srgbColor = glm::vec3(newColor.redF(), newColor.greenF(), newColor.blueF());
    setColor(glm::convertSRGBToLinear(srgbColor));
  }
}

void ColorWidget::setColor(const glm::vec3& newValue) noexcept
{
  if(_color == newValue) return;
  _color = newValue;
  _updateColorWidget();
  onColorChanged(newValue);
  emit colorChanged(newValue);
}

void ColorWidget::_updateColorWidget() noexcept
{
  glm::vec3 srgbColor = glm::convertLinearToSRGB(_color);
  QColor currentColor;
  currentColor.setRgbF(srgbColor.r, srgbColor.g, srgbColor.b);

  QPalette palette = _ui->colorFrame->palette();
  palette.setColor(QPalette::Window, currentColor);
  _ui->colorFrame->setPalette(palette);
}

void ColorWidget::onColorChanged(const glm::vec3& newValue) noexcept
{
}
