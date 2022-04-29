#include <mtt/clPipeline/constants.h>

#include <Render/Particles/ParticlesShadowmapTechnique.h>
#include <Render/Particles/ShadowDrawParticlesAction.h>

ParticlesShadowmapTechnique::ParticlesShadowmapTechnique(
                                          ParticlesDrawCommonData& commonData) :
  ParticlesAbstractTechnique( commonData,
                              mtt::clPipeline::transparentShadowmapStage)
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
  vertexShader->setDefine("MODIFICATOR_DECLARATION", "");
  vertexShader->setDefine("APPLY_POSTEFFECT", "");
  vertexShader->setDefine("APPLY_AMBIENT_WEIGHT", "");
  vertexShader->setDefine("APPLY_LIGHT", "");
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
  fragmentShader->newFragment().loadFromFile("particles.frag");
  pipeline.addShader(std::move(fragmentShader));

  pipeline.setDefine("SHADOWMAP_OUTPUT");

  pipeline.addResource( "nearfarBinding",
                        _nearFarUniform,
                        VK_SHADER_STAGE_GEOMETRY_BIT);
}

void ParticlesShadowmapTechnique::buildDrawAction(
                      mtt::DrawBin& renderBin,
                      mtt::DrawPlanBuildInfo& buildInfo,
                      mtt::GraphicsPipeline& pipeline,
                      uint32_t pointsNumber,
                      mtt::PlainBuffer& indices,
                      mtt::VolatileUniform<mtt::DrawMatrices>& matricesUniform)
{
  NearFarInfo nearFar;
  nearFar.nearDistance = buildInfo.currentViewInfo.nearCameraDistance;
  nearFar.nearFarDistance =
            buildInfo.currentViewInfo.farCameraDistance - nearFar.nearDistance;

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
                              nearFar);
}
