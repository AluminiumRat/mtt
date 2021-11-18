#pragma once

#include <memory>
#include <vector>

#include <mtt/Render/DrawPlan/AbstractFramePlan.h>

namespace mtt
{
  class AbstractFrameBuilder;
  class CommandProducer;

  class DrawPlan
  {
  public:
    DrawPlan() = default;
    DrawPlan(const DrawPlan&) = delete;
    DrawPlan& operator = (const DrawPlan&) = delete;
    ~DrawPlan() = default;

    void addFramePlan(std::unique_ptr<AbstractFramePlan> framePlan);
    std::vector<AbstractFramePlan*> findPlans(
                                          const AbstractFrameBuilder& builder);

    void addDependency( const AbstractFramePlan& sourceFrame,
                        const AbstractFramePlan& dependentFrame);
  
    void scheduleRender(CommandProducer& drawProducer);

  private:
    struct FrameRecord
    {
      std::unique_ptr<AbstractFramePlan> plan = nullptr;
      bool executed = false;
      bool visited = false;
      std::vector<size_t> sourceFrameIndices;

      FrameRecord() = default;
      FrameRecord(const FrameRecord&) = delete;
      FrameRecord& operator = (const FrameRecord&) = delete;
      FrameRecord(FrameRecord&&) = default;
      ~FrameRecord() noexcept = default;
    };

  private:
    void _scheduleFrame(FrameRecord& record, CommandProducer& drawProducer);

  private:
    using FrameTable = std::vector<FrameRecord>;
    FrameTable _frameTable;
  };
}