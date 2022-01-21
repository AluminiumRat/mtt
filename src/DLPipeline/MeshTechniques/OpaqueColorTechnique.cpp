#include <mtt/DLPipeline/MeshTechniques/OpaqueColorTechnique.h>
#include <mtt/DLPipeline/constants.h>
#include <mtt/Render/DrawPlan/DrawMeshAction.h>
#include <mtt/Render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/Render/Mesh/Mesh.h>

using namespace mtt;
using namespace mtt::DLPipeline;

OpaqueColorTechnique::OpaqueColorTechnique(VkPrimitiveTopology topology) :
  BaseGeometryTechnique(NORMALS_FEATURE |
                          MATERIAL_FEATURE |
                          TEX_COORDS_FEATURE |
                          SKELETON_FEATURE |
                          ALBEDO_SAMPLER_FEATURE |
                          SPECULAR_SAMPLER_FEATURE |
                          NORMAL_SAMPLER_FEATURE,
                        gBufferStage,
                        topology,
                        NEAR_FIRST_ORDER)
{
}

void OpaqueColorTechnique::adjustPipeline(GraphicsPipeline& pipeline,
                                          AbstractRenderPass& renderPass)
{
  BaseGeometryTechnique::adjustPipeline(pipeline, renderPass);

  pipeline.setDepthTestEnable(true);
  pipeline.setDepthWriteEnable(true);
  pipeline.setDepthCompareOp(VK_COMPARE_OP_GREATER_OR_EQUAL);

  std::unique_ptr<ShaderModule> fragmentShader(
                                new ShaderModule( ShaderModule::FRAGMENT_SHADER,
                                                  renderPass.device()));
  fragmentShader->newFragment().loadFromFile("DLPipeline/meshGBuffer.frag");
  pipeline.addShader(std::move(fragmentShader));
}

void OpaqueColorTechnique::createRenderAction(
                                            DrawPlanBuildInfo& buildInfo,
                                            GraphicsPipeline& pipeline,
                                            MatricesUniform& matricesUniform)
{
  if(!normalsEnabled()) return;

  BaseGeometryTechnique::createRenderAction(buildInfo,
                                            pipeline,
                                            matricesUniform);
}
