#include <mtt/DLPipeline/MeshTechniques/ShadowmapMeshTechnique.h>
#include <mtt/DLPipeline/constants.h>
#include <mtt/Render/DrawPlan/DrawMeshAction.h>
#include <mtt/Render/DrawPlan/DrawPlanBuildInfo.h>

using namespace mtt;
using namespace mtt::DLPipeline;

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
  fragmentShader->newFragment().loadFromFile("DLPipeline/meshShadowmap.frag");
  pipeline.addShader(std::move(fragmentShader));

  pipeline.addResource( "nearfarBindong",
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
