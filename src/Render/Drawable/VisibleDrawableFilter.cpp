#include <mtt/Render/Drawable/VisibleDrawableFilter.h>

using namespace mtt;

VisibleDrawableFilter::VisibleDrawableFilter() :
  _visible(true)
{
}

void VisibleDrawableFilter::setVisible(bool newValue) noexcept
{
  _visible = newValue;
}

void VisibleDrawableFilter::draw( DrawPlanBuildInfo& buildInfo,
                                  Modificator** next,
                                  size_t modifiactorsLeft,
                                  Drawable& drawable) const
{
  if(!_visible) return;
  drawNext(buildInfo, next, modifiactorsLeft, drawable);
}
