#include <mtt/render/Drawable/BoneMatricesSetter.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>

using namespace mtt;

void BoneMatricesSetter::draw(DrawPlanBuildInfo& buildInfo,
                              Modificator** next,
                              size_t modifiactorsLeft,
                              Drawable& drawable) const
{
  const std::vector<glm::mat4>* tmp = buildInfo.boneMatrices;
  if(matrices.empty()) buildInfo.boneMatrices = nullptr;
  else buildInfo.boneMatrices = &matrices;

  drawNext(buildInfo, next, modifiactorsLeft, drawable);

  buildInfo.boneMatrices = tmp;
}
