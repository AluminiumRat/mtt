#pragma once

#include <mtt/render/RenderPass/AbstractRenderPass.h>

namespace mtt
{
  class AbstractFramePlan;
  struct DrawContext;

  class GeneralRenderPass : public AbstractRenderPass
  {
  public:
    explicit GeneralRenderPass(LogicalDevice& device);
    GeneralRenderPass(const GeneralRenderPass&) = delete;
    GeneralRenderPass& operator = (const GeneralRenderPass&) = delete;
  protected:
    virtual ~GeneralRenderPass() noexcept = default;
  
  public:
    /// The frame buffer in draw context should be created by this render pass
    virtual void scheduleRender(AbstractFramePlan& plan, DrawContext& context);
  };
};