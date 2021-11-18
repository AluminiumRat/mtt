#pragma once

#include <glm/vec3.hpp>

#include <QtWidgets/QWidget>

namespace Ui
{
  class ColorPropertyWidget;
}

namespace mtt
{
  class ColorWidget : public QWidget
  {
    Q_OBJECT
  
  public:
    ColorWidget();
    ColorWidget(const ColorWidget&) = delete;
    ColorWidget& operator = (const ColorWidget&) = delete;
    virtual ~ColorWidget() noexcept;

    inline const glm::vec3& color() const noexcept;
    void setColor(const glm::vec3& newValue) noexcept;

  signals:
    void colorChanged(const glm::vec3& newValue);

  protected:
    virtual void onColorChanged(const glm::vec3& newValue) noexcept;
  
  private:
    void _updateColorWidget() noexcept;
    void _changeColor() noexcept;

  private:
    Ui::ColorPropertyWidget* _ui;
    glm::vec3 _color;
  };

  inline const glm::vec3& ColorWidget::color() const noexcept
  {
    return _color;
  }
}