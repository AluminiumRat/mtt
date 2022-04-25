#include <mtt/application/Widgets/RenderWidget/RenderWidget.h>
#include <mtt/editorLib/MainWindow/RenderMenu.h>
#include <mtt/utilities/ScopedSetter.h>

using namespace mtt;

RenderMenu::RenderMenu(RenderWidget& renderWidget) :
  QMenu(tr("&Render")),
  _renderWidget(renderWidget),
  _skipUpdate(false),
  _limitedFPSAction(nullptr)
{
  addAction(tr("&Statistic"), this, &RenderMenu::_showStatistic);

  _limitedFPSAction = addAction(tr("&Limited fps"),
                                this,
                                &RenderMenu::_updateFPSLimitFromAction);
  _limitedFPSAction->setCheckable(true);

  connect(&_renderWidget,
          &RenderWidget::presentModeChanged,
          this,
          &RenderMenu::_updateFPSLimitFromWidget,
          Qt::DirectConnection);
  _updateFPSLimitFromWidget();
}

void RenderMenu::_showStatistic() noexcept
{
  _statisticWidget.show();
  _statisticWidget.setFixedSize(_statisticWidget.size());
}

void RenderMenu::_updateFPSLimitFromWidget() noexcept
{
  if(_skipUpdate) return;
  ScopedSetter<bool> lock(_skipUpdate, true);

  _limitedFPSAction->setChecked(
                  _renderWidget.presentMode() == SwapChain::FIFO_PRESENT_MODE);
}

void RenderMenu::_updateFPSLimitFromAction() noexcept
{
  if(_skipUpdate) return;
  ScopedSetter<bool> lock(_skipUpdate, true);

  _renderWidget.setPresentMode(_limitedFPSAction->isChecked() ?
                                              SwapChain::FIFO_PRESENT_MODE :
                                              SwapChain::MAILBOX_PRESENT_MODE);
}
