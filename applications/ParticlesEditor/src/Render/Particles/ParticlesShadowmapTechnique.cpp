#include <mtt/clPipeline/constants.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>

#include <Render/Particles/ParticlesShadowmapTechnique.h>
#include <Render/Particles/ShadowDrawParticlesAction.h>

ParticlesShadowmapTechnique::ParticlesShadowmapTechnique(
                                          ParticlesDrawCommonData& commonData) :
  ParticlesAbstractTechnique( commonData,
                              mtt::clPipeline::transparentShadowmapStage,
                              3)
{
}

void ParticlesShadowmapTechnique::adjustPipeline(
                                                mtt::GraphicsPipeline& pipeline)
{
  pipeline.setDepthTestEnable(false);
  pipeline.setDepthWriteEnable(false);
  pipeline.setDepthCompareOp(VK_COMPARE_OP_GREATER_OR_EQUAL);

  mtt::LogicalDevice& device = pipeline.device();

  std::unique_ptr<mtt::ShaderModule> vertexShader(
                                      new mtt::ShaderModule(
                                              mtt::ShaderModule::VERTEX_SHADER,
                                              device));
  vertexShader->newFragment().loadFromFile("particlesShadow.vert");
  pipeline.addShader(std::move(vertexShader));

  std::unique_ptr<mtt::ShaderModule> geometryShader(
                                    new mtt::ShaderModule(
                                            mtt::ShaderModule::GEOMETRY_SHADER,
                                            device));
  geometryShader->newFragment().loadFromFile("particlesShadow.geom");
  pipeline.addShader(std::move(geometryShader));

  std::unique_ptr<mtt::ShaderModule> fragmentShader(
                                    new mtt::ShaderModule(
                                            mtt::ShaderModule::FRAGMENT_SHADER,
                                            device));
  fragmentShader->newFragment().loadFromFile("particlesShadow.frag");
  pipeline.addShader(std::move(fragmentShader));

  pipeline.addResource( "maxDistanceBinding",
                        _maxDistanceUniform,
                        VK_SHADER_STAGE_GEOMETRY_BIT);
}

void ParticlesShadowmapTechnique::buildDrawAction(
          mtt::DrawBin& renderBin,
          mtt::DrawPlanBuildInfo& buildInfo,
          mtt::GraphicsPipeline& pipeline,
          uint32_t pointsNumber,
          mtt::PlainBuffer& indices,
          mtt::VolatileUniform<mtt::DrawMatrices>& matricesUniform,
          mtt::VolatileUniform<mtt::MppxDistanceFunction>& mppxFunctionUniform,
          mtt::VolatileUniform<glm::vec2>& falloffUniform,
          glm::vec2 falloffValue)
{
  mtt::MppxDistanceFunction mppxFunction =
                                      buildInfo.currentViewInfo.mppxFunction();

  renderBin.createAction<ShadowDrawParticlesAction>(
                              buildInfo.getPriorityFarFirstOrder(glm::vec3(0)),
                              pipeline,
                              buildInfo.viewport,
                              buildInfo.scissor,
                              pointsNumber,
                              indices,
                              matricesUniform,
                              buildInfo.drawMatrices,
                              _maxDistanceUniform,
                              buildInfo.currentViewInfo.farCameraDistance,
                              mppxFunctionUniform,
                              mppxFunction,
                              falloffUniform,
                              falloffValue);
}
