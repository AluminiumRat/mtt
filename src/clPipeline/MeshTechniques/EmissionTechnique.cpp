#include <mtt/clPipeline/MeshTechniques/EmissionTechnique.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>

using namespace mtt;
using namespace mtt::clPipeline;

EmissionTechnique::EmissionTechnique(VkPrimitiveTopology topology) :
  BaseGeometryTechnique(MATERIAL_FEATURE |
                          TEX_COORDS_FEATURE |
                          SKELETON_FEATURE |
                          EMISSION_SAMPLER_FEATURE |
                          REFLECTION_SAMPLER_FEATURE,
                        emissionStage,
                        topology,
                        NEAR_FIRST_ORDER)
{
}

void EmissionTechnique::adjustPipeline( GraphicsPipeline& pipeline,
                                        AbstractRenderPass& renderPass)
{
  BaseGeometryTechnique::adjustPipeline(pipeline, renderPass);

  pipeline.setDepthTestEnable(true);
  pipeline.setDepthWriteEnable(false);
  pipeline.setDepthCompareOp(VK_COMPARE_OP_GREATER_OR_EQUAL);

  std::unique_ptr<ShaderModule> fragmentShader(
                                new ShaderModule( ShaderModule::FRAGMENT_SHADER,
                                                  renderPass.device()));
  fragmentShader->newFragment().loadFromFile("meshLib.frag");
  fragmentShader->newFragment().loadFromFile("clPipeline/meshEmission.frag");
  pipeline.addShader(std::move(fragmentShader));
}

void EmissionTechnique::createRenderAction( DrawPlanBuildInfo& buildInfo,
                                            GraphicsPipeline& pipeline,
                                            MatricesUniform& matricesUniform)
{
  if(!materialDataEnabled()) return;
  BaseGeometryTechnique::createRenderAction(buildInfo,
                                            pipeline,
                                            matricesUniform);
}
