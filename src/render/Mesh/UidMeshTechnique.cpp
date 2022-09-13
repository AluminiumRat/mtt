#include <mtt/render/DrawPlan/DrawMeshAction.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/Mesh/UidMeshTechnique.h>

using namespace mtt;

UidMeshTechnique::UidMeshTechnique( VkPrimitiveTopology topology,
                                    bool depthTestEnabled,
                                    bool depthWriteEnabled) :
  BaseGeometryTechnique(SKELETON_FEATURE,
                        baseStages::uidStage,
                        topology,
                        depthTestEnabled ? NEAR_FIRST_ORDER : FAR_FIRST_ORDER),
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
  glm::uvec2 uidValue(0);

  const UID* uidPtr = static_cast<const UID*>(
              buildInfo.extraData.data(DrawPlanBuildExtraData::objectUIDIndex));
  if (uidPtr != nullptr)
  {
    const uint64_t denom = 0x100000000ULL;
    uidValue = glm::uvec2(uidPtr->value() % denom, uidPtr->value() / denom);
  }

  createActionTemplate( buildInfo,
                        pipeline,
                        matricesUniform,
                        *_uidUniform,
                        uidValue);
}
