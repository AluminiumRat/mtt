#include <mtt/DLPipeline/MeshTechniques/InstrumentalTechnique.h>
#include <mtt/DLPipeline/constants.h>
#include <mtt/Render/DrawPlan/DrawMeshAction.h>
#include <mtt/Render/DrawPlan/DrawPlanBuildInfo.h>

using namespace mtt;
using namespace mtt::DLPipeline;

InstrumentalTechnique::InstrumentalTechnique( bool depthTestEnabled,
                                              bool depthWriteEnabled,
                                              VkCompareOp depthCompareOp,
                                              VkPrimitiveTopology topology) :
  BaseGeometryTechnique(MATERIAL_FEATURE |
                          TEX_COORDS_FEATURE |
                          ALBEDO_SAMPLER_FEATURE |
                          SKELETON_FEATURE,
                        instrumentStage,
                        topology,
                        FAR_FIRST_ORDER),
  _depthTestEnabled(depthTestEnabled),
  _depthWriteEnabled(depthWriteEnabled),
  _depthCompareOp(depthCompareOp)
{
}

void InstrumentalTechnique::adjustPipeline( GraphicsPipeline& pipeline,
                                            AbstractRenderPass& renderPass)
{
  BaseGeometryTechnique::adjustPipeline(pipeline, renderPass);

  pipeline.setDepthTestEnable(_depthTestEnabled);
  pipeline.setDepthWriteEnable(_depthWriteEnabled);
  pipeline.setDepthCompareOp(_depthCompareOp);

  std::unique_ptr<ShaderModule> fragmentShader(
                                new ShaderModule( ShaderModule::FRAGMENT_SHADER,
                                                  renderPass.device()));
  fragmentShader->newFragment().loadFromFile("DLPipeline/meshInstrument.frag");
  pipeline.addShader(std::move(fragmentShader));
}
