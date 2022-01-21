#include <mtt/render/DrawPlan/DrawMeshAction.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/Mesh/DepthMeshTechnique.h>

using namespace mtt;

DepthMeshTechnique::DepthMeshTechnique(VkPrimitiveTopology topology) :
  BaseGeometryTechnique(SKELETON_FEATURE,
                        baseStages::depthmapStage,
                        topology,
                        NEAR_FIRST_ORDER)
{
}

void DepthMeshTechnique::adjustPipeline(GraphicsPipeline& pipeline,
                                        AbstractRenderPass& renderPass)
{
  BaseGeometryTechnique::adjustPipeline(pipeline, renderPass);

  pipeline.setDepthTestEnable(true);
  pipeline.setDepthWriteEnable(true);
  pipeline.setDepthCompareOp(VK_COMPARE_OP_GREATER_OR_EQUAL);
}
