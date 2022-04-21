#include <mtt/render/Drawable/UidDrawableModificator.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>

using namespace mtt;

void UidDrawableModificator::setUid(UID newValue) noexcept
{
  _uid = newValue;
}

void UidDrawableModificator::draw(DrawPlanBuildInfo& buildInfo,
                                  DrawableModificator** next,
                                  size_t modifiactorsLeft,
                                  Drawable& drawable) const
{
  const void* oldUID =
              buildInfo.extraData.data(DrawPlanBuildExtraData::objectUIDIndex);

  try
  {
    buildInfo.extraData.setData(&_uid,
                                DrawPlanBuildExtraData::objectUIDIndex);
    drawNext(buildInfo, next, modifiactorsLeft, drawable);
  }
  catch (...)
  {
    buildInfo.extraData.setData(oldUID,
                                DrawPlanBuildExtraData::objectUIDIndex);
    throw;
  }

  buildInfo.extraData.setData(oldUID,
                              DrawPlanBuildExtraData::objectUIDIndex);
}
