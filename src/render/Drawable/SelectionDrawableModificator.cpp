#include <mtt/render/Drawable/SelectionDrawableModificator.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>

using namespace mtt;

SelectionDrawableModificator::SelectionDrawableModificator() :
  _selected(false)
{
}

void SelectionDrawableModificator::setSelected(bool newValue) noexcept
{
  _selected = newValue;
}

void SelectionDrawableModificator::draw(DrawPlanBuildInfo& buildInfo,
                                        Modificator** next,
                                        size_t modifiactorsLeft,
                                        Drawable& drawable) const
{
  bool oldSelected = buildInfo.selected;
  buildInfo.selected = _selected;
  drawNext(buildInfo, next, modifiactorsLeft, drawable);
  buildInfo.selected = oldSelected;
}
