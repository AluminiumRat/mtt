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
  mtt::LogicalDevice& device = pipeline.device();

  std::unique_ptr<mtt::ShaderModule> vertexShader(
                                      new mtt::ShaderModule(
                                              mtt::ShaderModule::VERTEX_SHADER,
                                              device));
  vertexShader->newFragment().loadFromFile("particles.vert");
  pipeline.addShader(std::move(vertexShader));

  std::unique_ptr<mtt::ShaderModule> geometryShader(
                                    new mtt::ShaderModule(
                                            mtt::ShaderModule::GEOMETRY_SHADER,
                                            device));
  geometryShader->newFragment().loadFromFile("particles.geom");
  pipeline.addShader(std::move(geometryShader));

  std::unique_ptr<mtt::ShaderModule> fragmentShader(
                                    new mtt::ShaderModule(
                                            mtt::ShaderModule::FRAGMENT_SHADER,
                                            device));
  fragmentShader->newFragment().loadFromFile("particlesShadow.frag");
  pipeline.addShader(std::move(fragmentShader));

  pipeline.addResource( "nearfarBinding",
                        _nearFarUniform,
                        VK_SHADER_STAGE_GEOMETRY_BIT);

  pipeline.setDefine("SHADOWMAP_OUTPUT");
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
  NearFarInfo nearFar;
  nearFar.nearDistance = buildInfo.currentViewInfo.nearCameraDistance;
  nearFar.nearFarDistance =
            buildInfo.currentViewInfo.farCameraDistance - nearFar.nearDistance;

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
                              _nearFarUniform,
                              nearFar,
                              mppxFunctionUniform,
                              mppxFunction,
                              falloffUniform,
                              falloffValue);
}
