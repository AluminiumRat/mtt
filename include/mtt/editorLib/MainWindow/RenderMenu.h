#pragma once

#include <QtWidgets/QMenu>

#include <mtt/application/Widgets/StatisticWidget.h>

namespace mtt
{
  class RenderWidget;

  class RenderMenu : public QMenu
  {
    Q_OBJECT

  public:
    explicit RenderMenu(RenderWidget& renderWidget);
    RenderMenu(const RenderMenu&) = delete;
    RenderMenu& operator = (const RenderMenu&) = delete;
    virtual ~RenderMenu() noexcept = default;

  private:
    void _showStatistic() noexcept;
    void _updateFPSLimitFromWidget() noexcept;
    void _updateFPSLimitFromAction() noexcept;

  private:
    RenderWidget& _renderWidget;

    StatisticWidget _statisticWidget;

    bool _skipUpdate;
    QAction* _limitedFPSAction;
  };
}