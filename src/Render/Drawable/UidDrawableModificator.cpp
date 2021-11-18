#include <mtt/Render/Drawable/UidDrawableModificator.h>
#include <mtt/Render/DrawPlan/DrawPlanBuildInfo.h>

using namespace mtt;

void UidDrawableModificator::setUid(UID newValue) noexcept
{
  _uid = newValue;
}

void UidDrawableModificator::draw(DrawPlanBuildInfo& buildInfo,
                                  Modificator** next,
                                  size_t modifiactorsLeft,
                                  Drawable& drawable) const
{
  UID oldUid = buildInfo.objectUid;
  buildInfo.objectUid = _uid;
  drawNext(buildInfo, next, modifiactorsLeft, drawable);
  buildInfo.objectUid = oldUid;
}
