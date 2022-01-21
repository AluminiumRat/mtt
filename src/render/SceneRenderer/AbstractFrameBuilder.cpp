#include <mtt/render/RenderPass/GeneralRenderPass.h>
#include <mtt/render/SceneRenderer/AbstractFrameBuilder.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

AbstractFrameBuilder::AbstractFrameBuilder(FrameType frameType) :
  _frameType(frameType),
  _stagePasses{}
{
}

void AbstractFrameBuilder::registerStagePass(GeneralRenderPass& pass)
{
  for(size_t subpassIndex = 0;
      subpassIndex < pass.subpassesNumber();
      subpassIndex++)
  {
    for(StageIndex stage : pass.subpassStages(subpassIndex))
    {
      if(stage == baseStages::subsidiaryStage) continue;
      if(_stagePasses.size() <= stage.index())
      {
        _stagePasses.resize(size_t(stage.index() + 1), nullptr);
      }
      if(_stagePasses[stage.index()] != nullptr) Abort("AbstractFrameBuilder::registerStagePass: multiple passes or subpasses for one stage.");
      _stagePasses[stage.index()] = &pass;
    }
  }
}
