#include <mtt/Render/DrawPlan/DrawMeshAction.h>
#include <mtt/Render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/Render/Mesh/UidMeshTechnique.h>

using namespace mtt;

UidMeshTechnique::UidMeshTechnique( VkPrimitiveTopology topology,
                                    bool depthTestEnabled,
                                    bool depthWriteEnabled) :
  BaseGeometryTechnique(SKELETON_FEATURE,
                        baseStages::uidStage,
                        topology,
                        _depthTestEnabled ? NEAR_FIRST_ORDER : FAR_FIRST_ORDER),
  _depthTestEnabled(depthTestEnabled),
  _depthWriteEnabled(depthWriteEnabled)
{
}

void UidMeshTechnique::invalidatePipeline() noexcept
{
  _uidUniform.reset();
  BaseGeometryTechnique::invalidatePipeline();
}

void UidMeshTechnique::adjustPipeline(GraphicsPipeline& pipeline,
                                      AbstractRenderPass& renderPass)
{
  BaseGeometryTechnique::adjustPipeline(pipeline, renderPass);

  pipeline.setDepthTestEnable(_depthTestEnabled);
  pipeline.setDepthWriteEnable(_depthWriteEnabled);
  pipeline.setDepthCompareOp(VK_COMPARE_OP_GREATER_OR_EQUAL);

  std::unique_ptr<ShaderModule> fragmentShader(
                                new ShaderModule( ShaderModule::FRAGMENT_SHADER,
                                                  renderPass.device()));
  fragmentShader->newFragment().loadFromFile("meshUid.frag");
  pipeline.addShader(std::move(fragmentShader));

  _uidUniform.emplace();
  pipeline.addResource( "uidBinding",
                        _uidUniform.value(),
                        VK_SHADER_STAGE_FRAGMENT_BIT);
}

void UidMeshTechnique::createRenderAction(DrawPlanBuildInfo& buildInfo,
                                          GraphicsPipeline& pipeline,
                                          MatricesUniform& matricesUniform)
{
  const uint64_t denom = 0x100000000ULL;
  glm::uvec2 uidValue(buildInfo.objectUid.value() % denom,
                      buildInfo.objectUid.value() / denom);

  createActionTemplate( buildInfo,
                        pipeline,
                        matricesUniform,
                        *_uidUniform,
                        uidValue);
}
