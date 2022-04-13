#include <mtt/editorLib/RenderMenu.h>

using namespace mtt;

RenderMenu::RenderMenu() :
  QMenu(tr("&Render"))
{
  addAction(tr("&Statistic"), this, &RenderMenu::_showStatistic);
}

void RenderMenu::_showStatistic() noexcept
{
  _statisticWidget.show();
  _statisticWidget.setFixedSize(_statisticWidget.size());
}
