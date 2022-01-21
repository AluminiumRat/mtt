#pragma once

#include <QtWidgets/QWidget>

namespace Ui
{
  class StatisticWidget;
}

namespace mtt
{
  class StatisticWidget : public QWidget
  {
    Q_OBJECT
  
  public:
    StatisticWidget(QWidget* parent = nullptr);
    StatisticWidget(const StatisticWidget&) = delete;
    StatisticWidget& operator = (const StatisticWidget&) = delete;
    virtual ~StatisticWidget() noexcept;

  protected:
    virtual void showEvent(QShowEvent* event) override;
    virtual void hideEvent(QHideEvent* event) override;

  private:
    void _update() noexcept;

  private:
    Ui::StatisticWidget* _ui;
  };
};