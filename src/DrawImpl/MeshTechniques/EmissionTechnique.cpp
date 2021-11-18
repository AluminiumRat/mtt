#include <mtt/DrawImpl/MeshTechniques/EmissionTechnique.h>
#include <mtt/DrawImpl/constants.h>
#include <mtt/Render/DrawPlan/DrawMeshAction.h>
#include <mtt/Render/DrawPlan/DrawPlanBuildInfo.h>

using namespace mtt;

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
  fragmentShader->newFragment().loadFromFile("DrawImpl/meshEmission.frag");
  pipeline.addShader(std::move(fragmentShader));
}

void EmissionTechnique::createRenderAction( DrawPlanBuildInfo& buildInfo,
                                            GraphicsPipeline& pipeline,
                                            MatricesUniform& matricesUniform,
                                            const BoneMatrices* boneMatricesPtr)
{
  if(!materialDataEnabled()) return;
  BaseGeometryTechnique::createRenderAction(buildInfo,
                                            pipeline,
                                            matricesUniform,
                                            boneMatricesPtr);
}
