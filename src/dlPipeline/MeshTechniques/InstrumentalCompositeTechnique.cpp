#include <mtt/dlPipeline/MeshTechniques/InstrumentalCompositeTechnique.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>

using namespace mtt;
using namespace mtt::dlPipeline;

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

  const bool* selectedPtr = static_cast<const bool*>(
          buildInfo.extraData.data(DrawPlanBuildExtraData::selectedFlagIndex));
  if (selectedPtr != nullptr && *selectedPtr == true)
  {
    _selectionTechnique.addToDrawPlan(buildInfo);
  }
}
