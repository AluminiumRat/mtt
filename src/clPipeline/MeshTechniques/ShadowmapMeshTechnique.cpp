#include <mtt/clPipeline/MeshTechniques/ShadowmapMeshTechnique.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/render/DrawPlan/DrawMeshAction.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>

using namespace mtt;
using namespace mtt::clPipeline;

ShadowmapMeshTechnique::ShadowmapMeshTechnique(VkPrimitiveTopology topology) :
  BaseGeometryTechnique(SKELETON_FEATURE |
                          VIEW_COORDS_IN_FRAGMENT_SHADER_FEATURE,
                        shadowmapStage,
                        topology,
                        NEAR_FIRST_ORDER)
{
}

void ShadowmapMeshTechnique::adjustPipeline(GraphicsPipeline& pipeline,
                                            AbstractRenderPass& renderPass)
{
  BaseGeometryTechnique::adjustPipeline(pipeline, renderPass);

  pipeline.setDepthTestEnable(true);
  pipeline.setDepthWriteEnable(true);
  pipeline.setDepthCompareOp(VK_COMPARE_OP_GREATER_OR_EQUAL);

  std::unique_ptr<ShaderModule> fragmentShader(
                              new ShaderModule( ShaderModule::FRAGMENT_SHADER,
                                                renderPass.device()));
  fragmentShader->newFragment().loadFromFile("clPipeline/meshShadowmap.frag");
  pipeline.addShader(std::move(fragmentShader));

  pipeline.addResource( "nearfarBinding",
                        _nearFarUniform,
                        VK_SHADER_STAGE_FRAGMENT_BIT);
}

void ShadowmapMeshTechnique::createRenderAction(
                                              DrawPlanBuildInfo& buildInfo,
                                              GraphicsPipeline& pipeline,
                                              MatricesUniform& matricesUniform)
{
  NearFarInfo nearFar;
  nearFar.nearDistance = buildInfo.currentViewInfo.nearCameraDistance;
  nearFar.nearFarDistance =
            buildInfo.currentViewInfo.farCameraDistance - nearFar.nearDistance;

  createActionTemplate( buildInfo,
                        pipeline,
                        matricesUniform,
                        _nearFarUniform,
                        nearFar);
}
