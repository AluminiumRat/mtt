#pragma once

#include <optional>

#include <mtt/render/Drawable/Drawable.h>
#include <mtt/render/DrawPlan/StageIndex.h>
#include <mtt/render/Pipeline/GraphicsPipeline.h>
#include <mtt/render/SceneRenderer/FrameType.h>

namespace mtt
{
  class PipelineDrawable : public Drawable
  {
  public:
    PipelineDrawable(FrameType frameType, StageIndex stageIndex) noexcept;
    PipelineDrawable(const PipelineDrawable&) = delete;
    PipelineDrawable& operator = (const PipelineDrawable&) = delete;
    virtual ~PipelineDrawable() noexcept = default;

    virtual void resetPipeline() noexcept;

    virtual void addToDrawPlan(DrawPlanBuildInfo& buildInfo) override;

    inline bool initialized() const noexcept;
    inline FrameType frameType() const noexcept;
    inline StageIndex stageIndex() const noexcept;

    inline LogicalDevice& device() const noexcept;

  protected:
    virtual void adjustPipeline(GraphicsPipeline& pipeline) = 0;
    inline const GraphicsPipeline* pipeline() const noexcept;
    inline GraphicsPipeline* pipeline() noexcept;

  private:
    bool _initialized;
    FrameType _frameType;
    StageIndex _stageIndex;
    std::optional<GraphicsPipeline> _pipeline;
  };

  inline bool PipelineDrawable::initialized() const noexcept
  {
    return _initialized;
  }

  inline FrameType PipelineDrawable::frameType() const noexcept
  {
    return _frameType;
  }

  inline StageIndex PipelineDrawable::stageIndex() const noexcept
  {
    return _stageIndex;
  }

  inline const GraphicsPipeline* PipelineDrawable::pipeline() const noexcept
  {
    if(_pipeline.has_value()) return &_pipeline.value();
    return nullptr;
  }

  inline GraphicsPipeline* PipelineDrawable::pipeline() noexcept
  {
    if (_pipeline.has_value()) return &_pipeline.value();
    return nullptr;
  }
}