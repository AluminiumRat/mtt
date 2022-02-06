#include <algorithm>

#include <mtt/render/DrawPlan/DrawBin.h>

using namespace mtt;

DrawBin::DrawBin(ActionMemoryPool& memoryPool) :
  _memoryPool(memoryPool)
{
}

void DrawBin::execute(DrawContext& drawContext)
{
  std::sort(_queue.begin(),
            _queue.end(),
            [](const ActionRecord& x, const ActionRecord& y) -> bool
            {
              return x.priority > y.priority;
            });

  for(ActionRecord& record : _queue)
  {
    record.action->execute(drawContext);
  }
}
