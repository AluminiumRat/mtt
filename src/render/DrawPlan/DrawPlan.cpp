#include <mtt/render/DrawPlan/DrawPlan.h>
#include <mtt/render/SceneRenderer/AbstractFrame.h>
#include <mtt/render/SceneRenderer/AbstractFrameBuilder.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

void DrawPlan::addFramePlan(std::unique_ptr<AbstractFramePlan> framePlan)
{
  FrameRecord newRecord;
  newRecord.plan = std::move(framePlan);
  newRecord.executed = false;
  newRecord.visited= false;
  _frameTable.push_back(std::move(newRecord));
}

std::vector<AbstractFramePlan*> DrawPlan::findFramePlans(
                                            const AbstractFrameBuilder& builder)
{
  std::vector<AbstractFramePlan*> plans;
  
  for(FrameRecord& frame : _frameTable)
  {
    if(&frame.plan->frameBuilder() == &builder)
    {
      plans.push_back(frame.plan.get());
    }
  }
  return plans;
}

void DrawPlan::addDependency( const AbstractFramePlan& sourceFrame,
                              const AbstractFramePlan& dependentFrame)
{
  for(size_t sourceIndex = 0; sourceIndex < _frameTable.size(); sourceIndex++)
  {
    FrameRecord& source = _frameTable[sourceIndex];
    if(source.plan.get() == &sourceFrame)
    {
      for(FrameRecord& depndent : _frameTable)
      {
        if(depndent.plan.get() == &dependentFrame)
        {
          depndent.sourceFrameIndices.push_back(sourceIndex);
        }
      }
    }
  }
}

void DrawPlan::_scheduleFrame(FrameRecord& record,
                              CommandProducer& drawProducer)
{
  if(record.visited) Abort("DrawPlan::_scheduleFrame: looped frame graph.");
  record.visited = true;

  for(size_t sourceIndex : record.sourceFrameIndices)
  {
    FrameRecord& subframe = _frameTable[sourceIndex];
    if(!subframe.executed) _scheduleFrame(subframe, drawProducer);
  }

  AbstractFrame& frame = record.plan->frame();
  AbstractFrameBuilder& builder = frame.builder();
  builder.scheduleRender(*record.plan, drawProducer);

  record.executed = true;
  record.visited = false;
}

void DrawPlan::scheduleRender(CommandProducer& drawProducer)
{
  for(FrameRecord& frame : _frameTable)
  {
    frame.executed = false;
    frame.visited = false;
  }

  for(FrameRecord& frame : _frameTable)
  {
    if(!frame.executed) _scheduleFrame(frame, drawProducer);
  }
}
