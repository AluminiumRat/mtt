#include <mtt/clPipeline/MeshTechniques/SelectionMeshTechnique.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/render/DrawPlan/DrawMeshAction.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>

using namespace mtt;
using namespace mtt::clPipeline;

SelectionMeshTechnique::SelectionMeshTechnique(VkPrimitiveTopology topology) :
  BaseGeometryTechnique(SKELETON_FEATURE,
                        selectionStage,
                        topology,
                        NEAR_FIRST_ORDER)
{
}

void SelectionMeshTechnique::adjustPipeline(GraphicsPipeline& pipeline,
                                            AbstractRenderPass& renderPass)
{
  BaseGeometryTechnique::adjustPipeline(pipeline, renderPass);

  pipeline.setDepthTestEnable(true);
  pipeline.setDepthWriteEnable(true);
  pipeline.setDepthCompareOp(VK_COMPARE_OP_GREATER_OR_EQUAL);

  std::unique_ptr<ShaderModule> fragmentShader(
                                new ShaderModule( ShaderModule::FRAGMENT_SHADER,
                                                  renderPass.device()));
  fragmentShader->newFragment().loadFromFile("clPipeline/meshSelection.frag");
  pipeline.addShader(std::move(fragmentShader));
}
