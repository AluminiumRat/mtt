#pragma once

#include <memory>
#include <vector>

#include <mtt/render/DrawPlan/AbstractFramePlan.h>
#include <mtt/render/SceneRenderer/AbstractFrame.h>
#include <mtt/render/SceneRenderer/FrameType.h>

namespace mtt
{
  class GeneralRenderPass;

  class AbstractFrameBuilder
  {
  public:
    explicit AbstractFrameBuilder(FrameType frameType);
    AbstractFrameBuilder(const AbstractFrameBuilder&) = delete;
    AbstractFrameBuilder& operator = (const AbstractFrameBuilder&) = delete;
    virtual ~AbstractFrameBuilder() = default;

    inline FrameType frameType() const noexcept;

    /// Returns nullptr if the stage is not supported
    inline GeneralRenderPass* stagePass(StageIndex stage) noexcept;
    /// Returns nullptr if the stage is not supported
    inline const GeneralRenderPass* stagePass(StageIndex stage) const noexcept;

    /// The frame should be created by this frame renderer
    virtual std::unique_ptr<AbstractFramePlan> createFramePlan(
                                                AbstractFrame& frame) const = 0;

    /// The frame plan should be created by this frame renderer
    virtual void scheduleRender(AbstractFramePlan& plan,
                                CommandProducer& drawProducer) = 0;

  protected:
    /// Should be called from constructor
    void registerStagePass(GeneralRenderPass& pass);

  private:
    FrameType _frameType;
    std::vector<GeneralRenderPass*> _stagePasses;
  };

  inline FrameType AbstractFrameBuilder::frameType() const noexcept
  {
    return _frameType;
  }

  inline GeneralRenderPass* AbstractFrameBuilder::stagePass(
                                                      StageIndex stage) noexcept
  {
    if(_stagePasses.size() <= stage.index()) return nullptr;
    return _stagePasses[stage.index()];
  }

  inline const GeneralRenderPass* AbstractFrameBuilder::stagePass(
                                                StageIndex stage) const noexcept
  {
    if (_stagePasses.size() <= stage.index()) return nullptr;
    return _stagePasses[stage.index()];
  }
}