#include <mtt/clPipeline/Background/BackgroundAreaModificator.h>
#include <mtt/clPipeline/Lighting/LightAreaModificator.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/particles/Drawable/ColorDrawParticlesAction.h>
#include <mtt/particles/Drawable/ParticlesColorTechnique.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/SceneGraph/AreaModificatorSet.h>

using namespace mtt;

ParticlesColorTechnique::ParticlesColorTechnique(
                                          ParticlesDrawCommonData& commonData,
                                          AreaModificatorSet& modificatorSet) :
  ParticlesAbstractTechnique(commonData, clPipeline::forwardLightStage, 1),
  _modificatorSet(modificatorSet)
{
  for (AreaModificator* modificator : _modificatorSet.modificators())
  {
    if( modificator->type() ==
                            clPipeline::BackgroundAreaModificator::typeIndex ||
        modificator->type() == clPipeline::LightAreaModificator::typeIndex)
    {
      modificator->addConsumer(*this);
    }
  }
}

ParticlesColorTechnique::~ParticlesColorTechnique()
{
  for (AreaModificator* modificator : _modificatorSet.modificators())
  {
    if (modificator->type() ==
                            clPipeline::BackgroundAreaModificator::typeIndex ||
        modificator->type() == clPipeline::LightAreaModificator::typeIndex)
    {
      modificator->removeConsumer(*this);
    }
  }
}

void ParticlesColorTechnique::resetPipeline() noexcept
{
  clearPipeline();
}

void ParticlesColorTechnique::_applyAreaModifictors(ShaderModule& shader,
                                                    GraphicsPipeline& pipeline)
{
  shader.setDefine("MODIFICATOR_DECLARATION", "");
  shader.setDefine("APPLY_POSTEFFECT", "");
  shader.setDefine("APPLY_AMBIENT_WEIGHT", "");
  shader.setDefine("APPLY_LIGHT", "");

  for(size_t modificatorIndex = 0;
      modificatorIndex < _modificatorSet.modificators().size();
      modificatorIndex++)
  {
    AreaModificator& modificator =
                              *_modificatorSet.modificators()[modificatorIndex];
    if(modificator.type() == clPipeline::BackgroundAreaModificator::typeIndex)
    {
      clPipeline::BackgroundAreaModificator& backgroundModificator =
              static_cast<clPipeline::BackgroundAreaModificator&>(modificator);
      backgroundModificator.adjustPipeline(pipeline, shader, modificatorIndex);
    }

    if (modificator.type() == clPipeline::LightAreaModificator::typeIndex)
    {
      clPipeline::LightAreaModificator& lightModificator =
                    static_cast<clPipeline::LightAreaModificator&>(modificator);
      lightModificator.adjustPipeline(
                    pipeline,
                    shader,
                    clPipeline::LightAreaModificator::OVERALL_ILLUMINANCE_MODEL,
                    modificatorIndex);
    }
  }
}

void ParticlesColorTechnique::adjustPipeline(GraphicsPipeline& pipeline)
{
  pipeline.setDepthTestEnable(true);
  pipeline.setDepthWriteEnable(false);
  pipeline.setDepthCompareOp(VK_COMPARE_OP_GREATER_OR_EQUAL);

  LogicalDevice& device = pipeline.device();

  std::unique_ptr<ShaderModule> vertexShader(
                                          new ShaderModule(
                                                  ShaderModule::VERTEX_SHADER,
                                                  device));
  std::unique_ptr<ShaderModule> geometryShader(
                                        new ShaderModule(
                                                ShaderModule::GEOMETRY_SHADER,
                                                device));
  std::unique_ptr<ShaderModule> fragmentShader(
                                        new ShaderModule(
                                                ShaderModule::FRAGMENT_SHADER,
                                                device));

  if(commonData().lightingType ==
                                ParticlesDrawCommonData::PER_PARTICLE_LIGHTING)
  {
    vertexShader->newFragment().loadFromFile(
                          "clPipeline/particles/particlesVertexLighting.vert");
    _applyAreaModifictors(*vertexShader, pipeline);
    geometryShader->newFragment().loadFromFile(
                          "clPipeline/particles/particlesVertexLighting.geom");
    fragmentShader->newFragment().loadFromFile(
                          "clPipeline/particles/particlesVertexLighting.frag");
  }
  else
  {
    vertexShader->newFragment().loadFromFile(
                        "clPipeline/particles/particlesFragmentLighting.vert");
    geometryShader->newFragment().loadFromFile(
                        "clPipeline/particles/particlesFragmentLighting.geom");
    fragmentShader->newFragment().loadFromFile(
                        "clPipeline/particles/particlesFragmentLighting.frag");
    _applyAreaModifictors(*fragmentShader, pipeline);
  }
  pipeline.addShader(std::move(vertexShader));
  pipeline.addShader(std::move(geometryShader));
  pipeline.addShader(std::move(fragmentShader));

  pipeline.addResource( "depthSamplerBinding",
                        commonData().depthSampler,
                        VK_SHADER_STAGE_FRAGMENT_BIT);
}

void ParticlesColorTechnique::buildDrawAction(
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

  renderBin.createAction<ColorDrawParticlesAction>(
                              buildInfo.getPriorityFarFirstOrder(glm::vec3(0)),
                              pipeline,
                              buildInfo.viewport,
                              buildInfo.scissor,
                              pointsNumber,
                              indices,
                              matricesUniform,
                              buildInfo.drawMatrices,
                              mppxFunctionUniform,
                              mppxFunction,
                              falloffUniform,
                              falloffValue,
                              *commonData().depthSamplerTexture);
}
