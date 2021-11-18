#include <glm/gtx/transform.hpp>

#include <mtt/Render/Drawable/AutoscaleDrawableModificator.h>
#include <mtt/Render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/Render/SceneGraph/CameraNode.h>

using namespace mtt;

AutoscaleDrawableModificator::AutoscaleDrawableModificator() :
  _mode(PIXEL_SCALE_MODE)
{
}

void AutoscaleDrawableModificator::draw(DrawPlanBuildInfo& buildInfo,
                                        Modificator** next,
                                        size_t modifiactorsLeft,
                                        Drawable& drawable) const
{
  if(_mode == NO_SCALE_MODE)
  {
    drawNext(buildInfo, next, modifiactorsLeft, drawable);
    return;
  }

  DrawMatrices oldMatrices = buildInfo.drawMatrices;

  glm::vec3 viewSpacePoint = buildInfo.drawMatrices.localToViewMatrix *
                                                          glm::vec4(0, 0, 0, 1);
  if(_mode == VIEWPORT_SCALE_MODE)
  {
    glm::vec2 screenSize = buildInfo.rootViewInfo.screenSize(viewSpacePoint);
    float maxSize = glm::max(screenSize.x, screenSize.y);
    buildInfo.drawMatrices.localToViewMatrix *= glm::scale(glm::vec3(maxSize));
  }
  else
  {
    float scale = buildInfo.rootViewInfo.mppx(viewSpacePoint);
    buildInfo.drawMatrices.localToViewMatrix *= glm::scale(glm::vec3(scale));
  }

  drawNext(buildInfo, next, modifiactorsLeft, drawable);

  buildInfo.drawMatrices = oldMatrices;
}

float AutoscaleDrawableModificator::scale(const glm::vec3& position,
                                          const ViewInfo& viewInfo,
                                          Mode mode) noexcept
{
  if(mode == NO_SCALE_MODE) return 1.f;

  if(mode == VIEWPORT_SCALE_MODE)
  {
    glm::vec2 screenSize = viewInfo.screenSize(position);
    return glm::max(screenSize.x, screenSize.y);
  }
  else
  {
    return viewInfo.mppx(position);
  }
}
