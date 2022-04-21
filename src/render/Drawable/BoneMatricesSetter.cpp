#include <mtt/render/Drawable/BoneMatricesSetter.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>

using namespace mtt;

void BoneMatricesSetter::draw(DrawPlanBuildInfo& buildInfo,
                              DrawableModificator** next,
                              size_t modifiactorsLeft,
                              Drawable& drawable) const
{
  const void* oldMatrices = buildInfo.extraData.data(
                                    DrawPlanBuildExtraData::boneMatricesIndex);

  try
  {
    if(matrices.empty())
    {
      buildInfo.extraData.setData(nullptr,
                                  DrawPlanBuildExtraData::boneMatricesIndex);
    }
    else
    {
      buildInfo.extraData.setData(&matrices,
                                  DrawPlanBuildExtraData::boneMatricesIndex);
    }
    drawNext(buildInfo, next, modifiactorsLeft, drawable);
  }
  catch (...)
  {
    buildInfo.extraData.setData(oldMatrices,
                                DrawPlanBuildExtraData::boneMatricesIndex);
    throw;
  }

  buildInfo.extraData.setData(oldMatrices,
                              DrawPlanBuildExtraData::boneMatricesIndex);
}
