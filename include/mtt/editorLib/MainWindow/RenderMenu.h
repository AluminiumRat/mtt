#pragma once

#include <QtWidgets/QMenu>

#include <mtt/application/Widgets/StatisticWidget.h>

namespace mtt
{
  class RenderMenu : public QMenu
  {
    Q_OBJECT

  public:
    RenderMenu();
    RenderMenu(const RenderMenu&) = delete;
    RenderMenu& operator = (const RenderMenu&) = delete;
    virtual ~RenderMenu() noexcept = default;

  private:
    void _showStatistic() noexcept;

  private:
    StatisticWidget _statisticWidget;
  };
}