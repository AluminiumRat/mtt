#include <mtt/DrawImpl/MeshTechniques/InstrumentalCompositeTechnique.h>
#include <mtt/Render/DrawPlan/DrawPlanBuildInfo.h>

using namespace mtt;

InstrumentalCompositeTechnique::InstrumentalCompositeTechnique(
                                                VkPrimitiveTopology topology,
                                                bool depthTestEnabled,
                                                bool depthWriteEnabled) :
  _colorTechnique(depthTestEnabled,
                  depthWriteEnabled,
                  VK_COMPARE_OP_GREATER_OR_EQUAL,
                  topology),
  _selectionTechnique(topology)
{
  registerSubtechnique(_colorTechnique);
  registerSubtechnique(_selectionTechnique);
}

void InstrumentalCompositeTechnique::addToDrawPlan(DrawPlanBuildInfo& buildInfo)
{
  _colorTechnique.addToDrawPlan(buildInfo);
  if (buildInfo.selected) _selectionTechnique.addToDrawPlan(buildInfo);
}
