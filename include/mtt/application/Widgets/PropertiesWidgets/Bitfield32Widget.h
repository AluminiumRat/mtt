#pragma once

#include <memory>

#include <QtWidgets/QWidget>

class QCheckBox;

namespace Ui
{
  class Bitfield32Widget;
}

namespace mtt
{
  class Bitfield32Widget : public QWidget
  {
  Q_OBJECT

  public:
    Bitfield32Widget();
    Bitfield32Widget(const Bitfield32Widget&) = delete;
    Bitfield32Widget& operator = (const Bitfield32Widget&) = delete;
    virtual ~Bitfield32Widget() noexcept;

    inline uint32_t value() const noexcept;
    void setValue(uint32_t newValue) noexcept;

  signals:
    void valueChanged(uint32_t newValue);

  private:
    void _addCheckbox(QCheckBox& checkbox, size_t index);
    void _updateValue() noexcept;
    void _updateWidgets() noexcept;

  private:
    std::unique_ptr<Ui::Bitfield32Widget> _ui;
    QCheckBox* _checkboxes[32];
    uint32_t _value;
    bool _skipUpdate;
  };

  inline uint32_t Bitfield32Widget::value() const noexcept
  {
    return _value;
  }
}