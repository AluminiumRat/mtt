#include <mtt/Render/DrawPlan/DrawContext.h>
#include <mtt/Render/DrawPlan/AbstractFramePlan.h>
#include <mtt/Render/RenderPass/GeneralRenderPass.h>
#include <mtt/Utilities/Abort.h>

using namespace mtt;

GeneralRenderPass::GeneralRenderPass(LogicalDevice& device) :
  AbstractRenderPass(device)
{
}

void GeneralRenderPass::scheduleRender( AbstractFramePlan& plan,
                                        DrawContext& context)
{
  auto drawDelegate =
    [&](DrawContext& context, size_t subpassIndex)
    {
      for(StageIndex stage : subpassStages(subpassIndex))
      {
        DrawBin* renderBin = plan.getBin(stage);
        if (renderBin == nullptr)
        {
          Abort("GeneralRenderPass::scheduleRender: render bin is not supported.");
        }

        renderBin->scheduleDraw(context);
      }
    };

  scheduleDrawT(context, drawDelegate);
}
