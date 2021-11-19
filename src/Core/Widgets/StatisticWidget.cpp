#include <mtt/Core/Widgets/StatisticWidget.h>
#include <mtt/Core/Application.h>

#include <GeneratedFiles/ui_StatisticWidget.h>

using namespace mtt;

StatisticWidget::StatisticWidget(QWidget* parent) :
  QWidget(parent),
  _ui(new Ui::StatisticWidget)
{
  _ui->setupUi(this);
}

StatisticWidget::~StatisticWidget() noexcept
{
  delete _ui;
}

void StatisticWidget::showEvent(QShowEvent* event)
{
  connect(&Application::instance(),
          &Application::statisticUpdated,
          this,
          &StatisticWidget::_update,
          Qt::DirectConnection);
  _update();
}

void StatisticWidget::hideEvent(QHideEvent* event)
{
  disconnect( &Application::instance(),
              &Application::statisticUpdated,
              this,
              &StatisticWidget::_update);
}

void StatisticWidget::_update() noexcept
{
  const ApplicationStatistic& statistic = Application::instance().statistic();
  _ui->cpsValueLabel->setText(QString::number(statistic.cyclesPerSecond));
}
