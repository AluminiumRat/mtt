#include <algorithm>
#include <float.h>

#include <mtt/Render/Drawable/LODDrawableFilter.h>
#include <mtt/Render/DrawPlan/DrawPlanBuildInfo.h>

using namespace mtt;

LODDrawableFilter::LODDrawableFilter() :
  _minMppx(0),
  _maxMppx(FLT_MAX)
{
}

void LODDrawableFilter::setRange(float minMppx, float maxMppx) noexcept
{
  _minMppx = minMppx;
  _maxMppx = maxMppx;
}

void LODDrawableFilter::draw( DrawPlanBuildInfo& buildInfo,
                              Modificator** next,
                              size_t modifiactorsLeft,
                              Drawable& drawable) const
{
  glm::vec3 viewSpacePosition = buildInfo.viewSpacePoint(glm::vec3(0));
  float currentFrameMppx = buildInfo.currentViewInfo.mppx(viewSpacePosition);
  float rootFrameMppx = buildInfo.rootViewInfo.mppx(viewSpacePosition);
  float finalMppx = std::max(currentFrameMppx, rootFrameMppx);
  if(finalMppx >= _minMppx && finalMppx < _maxMppx)
  {
    drawNext(buildInfo, next, modifiactorsLeft, drawable);
  }
}
