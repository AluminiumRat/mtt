#include <mtt/clPipeline/MeshTechniques/UnlightedColorTechnique.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>

using namespace mtt;
using namespace mtt::clPipeline;

UnlightedColorTechnique::UnlightedColorTechnique(
                                                bool depthTestEnabled,
                                                bool depthWriteEnabled,
                                                VkCompareOp depthCompareOp,
                                                VkPrimitiveTopology topology) :
  BaseGeometryTechnique(MATERIAL_FEATURE |
                          TEX_COORDS_FEATURE |
                          ALBEDO_SAMPLER_FEATURE |
                          OPAQUE_SAMPLER_FEATURE |
                          SKELETON_FEATURE,
                        forwardLightStage,
                        topology,
                        FAR_FIRST_ORDER),
  _depthTestEnabled(depthTestEnabled),
  _depthWriteEnabled(depthWriteEnabled),
  _depthCompareOp(depthCompareOp)
{
}

void UnlightedColorTechnique::adjustPipeline( GraphicsPipeline& pipeline,
                                              AbstractRenderPass& renderPass)
{
  BaseGeometryTechnique::adjustPipeline(pipeline, renderPass);

  pipeline.setDepthTestEnable(_depthTestEnabled);
  pipeline.setDepthWriteEnable(_depthWriteEnabled);
  pipeline.setDepthCompareOp(_depthCompareOp);

  std::unique_ptr<ShaderModule> fragmentShader(
                                new ShaderModule( ShaderModule::FRAGMENT_SHADER,
                                                  renderPass.device()));
  fragmentShader->newFragment().loadFromFile("clPipeline/meshUnlighted.frag");
  pipeline.addShader(std::move(fragmentShader));

  pipeline.setDefine("ENABLE_ALPHA_TEST");
}
