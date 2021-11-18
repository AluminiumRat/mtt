#pragma once

#include <QtWidgets/QDoubleSpinBox>

namespace Ui
{
  class Vec3PropertyWidget;
}

namespace mtt
{
  class Vec3Widget : public QWidget
  {
  public:
    Vec3Widget();
    Vec3Widget(const Vec3Widget&) = delete;
    Vec3Widget& operator = (const Vec3Widget&) = delete;
    virtual ~Vec3Widget() noexcept;

    inline QDoubleSpinBox& xSpin() noexcept;
    inline const QDoubleSpinBox& xSpin() const noexcept;

    inline QDoubleSpinBox& ySpin() noexcept;
    inline const QDoubleSpinBox& ySpin() const noexcept;

    inline QDoubleSpinBox& zSpin() noexcept;
    inline const QDoubleSpinBox& zSpin() const noexcept;
  
  private:
    Ui::Vec3PropertyWidget* _ui;
    QDoubleSpinBox* _xSpin;
    QDoubleSpinBox* _ySpin;
    QDoubleSpinBox* _zSpin;
  };

  inline QDoubleSpinBox& Vec3Widget::xSpin() noexcept
  {
    return *_xSpin;
  }

  inline const QDoubleSpinBox& Vec3Widget::xSpin() const noexcept
  {
    return *_xSpin;
  }

  inline QDoubleSpinBox& Vec3Widget::ySpin() noexcept
  {
    return *_ySpin;
  }

  inline const QDoubleSpinBox& Vec3Widget::ySpin() const noexcept
  {
    return *_ySpin;
  }

  inline QDoubleSpinBox& Vec3Widget::zSpin() noexcept
  {
    return *_zSpin;
  }

  inline const QDoubleSpinBox& Vec3Widget::zSpin() const noexcept
  {
    return *_zSpin;
  }
}