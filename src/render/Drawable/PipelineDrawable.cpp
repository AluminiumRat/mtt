#include <stdexcept>

#include <mtt/render/Drawable/PipelineDrawable.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/RenderPass/GeneralRenderPass.h>
#include <mtt/utilities/Log.h>

using namespace mtt;

PipelineDrawable::PipelineDrawable( FrameType frameType,
                                    StageIndex stageIndex) noexcept :
  _initialized(false),
  _frameType(frameType),
  _stageIndex(stageIndex)
{
}

void PipelineDrawable::resetPipeline() noexcept
{
  _initialized = false;
  _pipeline.reset();
}

void PipelineDrawable::addToDrawPlan(DrawPlanBuildInfo& buildInfo)
{
  if (buildInfo.frameType != _frameType) return;

  AbstractRenderPass* renderPass = buildInfo.builder->stagePass(_stageIndex);
  if (renderPass == nullptr) return;

  if (_pipeline.has_value() && !_pipeline->isCompatible(*renderPass))
  {
    resetPipeline();
  }

  if (!_pipeline.has_value())
  {
    if(_initialized) return;
    _initialized = true;

    try
    {
      _pipeline.emplace(*renderPass, _stageIndex);
      adjustPipeline(*_pipeline);
    }
    catch (std::exception& error)
    {
      resetPipeline();
      _initialized = true;
      Log() << "PipelineDrawable::addToDrawPlan: " << error.what();
      return;
    }
    catch (...)
    {
      resetPipeline();
      _initialized = true;
      Log() << "PipelineDrawable::addToDrawPlan: unknown error";
      return;
    }
  }

  Drawable::addToDrawPlan(buildInfo);
}
