#include <mtt/clPipeline/Particles/ParticlesShadowmapTechnique.h>
#include <mtt/clPipeline/Particles/ShadowDrawParticlesAction.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>

using namespace mtt;
using namespace mtt::clPipeline;

ParticlesShadowmapTechnique::ParticlesShadowmapTechnique(
                                          ParticlesDrawCommonData& commonData) :
  ParticlesAbstractTechnique(commonData, shadowmapStage, 3)
{
}

void ParticlesShadowmapTechnique::adjustPipeline(GraphicsPipeline& pipeline)
{
  pipeline.setDepthTestEnable(false);
  pipeline.setDepthWriteEnable(false);
  pipeline.setDepthCompareOp(VK_COMPARE_OP_GREATER_OR_EQUAL);

  LogicalDevice& device = pipeline.device();

  std::unique_ptr<ShaderModule> vertexShader(
                                  new ShaderModule( ShaderModule::VERTEX_SHADER,
                                                    device));
  vertexShader->newFragment().loadFromFile(
                                  "clPipeline/particles/particlesShadow.vert");
  pipeline.addShader(std::move(vertexShader));

  std::unique_ptr<ShaderModule> geometryShader(
                                new ShaderModule( ShaderModule::GEOMETRY_SHADER,
                                                  device));
  geometryShader->newFragment().loadFromFile(
                                  "clPipeline/particles/particlesShadow.geom");
  pipeline.addShader(std::move(geometryShader));

  std::unique_ptr<ShaderModule> fragmentShader(
                                new ShaderModule( ShaderModule::FRAGMENT_SHADER,
                                                  device));
  fragmentShader->newFragment().loadFromFile(
                                  "clPipeline/particles/particlesShadow.frag");
  pipeline.addShader(std::move(fragmentShader));

  pipeline.addResource( "maxDistanceBinding",
                        _maxDistanceUniform,
                        VK_SHADER_STAGE_GEOMETRY_BIT);
}

void ParticlesShadowmapTechnique::buildDrawAction(
          DrawBin& renderBin,
          DrawPlanBuildInfo& buildInfo,
          GraphicsPipeline& pipeline,
          uint32_t pointsNumber,
          PlainBuffer& indices,
          VolatileUniform<DrawMatrices>& matricesUniform,
          VolatileUniform<MppxDistanceFunction>& mppxFunctionUniform,
          VolatileUniform<glm::vec2>& falloffUniform,
          glm::vec2 falloffValue)
{
  MppxDistanceFunction mppxFunction =
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
