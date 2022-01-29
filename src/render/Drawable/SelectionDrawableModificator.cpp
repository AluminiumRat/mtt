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
                                        DrawableModificator** next,
                                        size_t modifiactorsLeft,
                                        Drawable& drawable) const
{
  const void* oldSelected = buildInfo.extraData.data(
                                    DrawPlanBuildExtraData::selectedFlagIndex);
  buildInfo.extraData.setData(&_selected,
                              DrawPlanBuildExtraData::selectedFlagIndex);
  drawNext(buildInfo, next, modifiactorsLeft, drawable);
  buildInfo.extraData.setData(oldSelected,
                              DrawPlanBuildExtraData::selectedFlagIndex);
}
