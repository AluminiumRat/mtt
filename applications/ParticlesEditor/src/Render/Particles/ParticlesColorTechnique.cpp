#include <mtt/clPipeline/Background/BackgroundAreaModificator.h>
#include <mtt/clPipeline/Lighting/LightAreaModificator.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/render/SceneGraph/AreaModificatorSet.h>

#include <Render/Particles/ColorDrawParticlesAction.h>
#include <Render/Particles/ParticlesColorTechnique.h>

ParticlesColorTechnique::ParticlesColorTechnique(
                                      ParticlesDrawCommonData& commonData,
                                      mtt::AreaModificatorSet& modificatorSet) :
  ParticlesAbstractTechnique( commonData,
                              mtt::clPipeline::forwardLightStage),
  _modificatorSet(modificatorSet)
{
  for (mtt::AreaModificator* modificator : _modificatorSet.modificators())
  {
    if( modificator->type() ==
                        mtt::clPipeline::BackgroundAreaModificator::typeIndex ||
        modificator->type() == mtt::clPipeline::LightAreaModificator::typeIndex)
    {
      modificator->addConsumer(*this);
    }
  }
}

ParticlesColorTechnique::~ParticlesColorTechnique()
{
  for (mtt::AreaModificator* modificator : _modificatorSet.modificators())
  {
    if (modificator->type() ==
                        mtt::clPipeline::BackgroundAreaModificator::typeIndex ||
        modificator->type() == mtt::clPipeline::LightAreaModificator::typeIndex)
    {
      modificator->removeConsumer(*this);
    }
  }
}

void ParticlesColorTechnique::resetPipeline() noexcept
{
  clearPipeline();
}

void ParticlesColorTechnique::_applyAreaModifictors(
                                                mtt::ShaderModule& shader,
                                                mtt::GraphicsPipeline& pipeline)
{
  shader.setDefine("MODIFICATOR_DECLARATION", "");
  shader.setDefine("APPLY_POSTEFFECT", "");
  shader.setDefine("APPLY_AMBIENT_WEIGHT", "");
  shader.setDefine("APPLY_LIGHT", "");

  for(size_t modificatorIndex = 0;
      modificatorIndex < _modificatorSet.modificators().size();
      modificatorIndex++)
  {
    mtt::AreaModificator& modificator =
                              *_modificatorSet.modificators()[modificatorIndex];
    if(modificator.type() ==
                          mtt::clPipeline::BackgroundAreaModificator::typeIndex)
    {
      mtt::clPipeline::BackgroundAreaModificator& backgroundModificator =
          static_cast<mtt::clPipeline::BackgroundAreaModificator&>(modificator);
      backgroundModificator.adjustPipeline(pipeline, shader, modificatorIndex);
    }

    if (modificator.type() == mtt::clPipeline::LightAreaModificator::typeIndex)
    {
      mtt::clPipeline::LightAreaModificator& lightModificator =
              static_cast<mtt::clPipeline::LightAreaModificator&>(modificator);
      lightModificator.adjustPipeline(
              pipeline,
              shader,
              mtt::clPipeline::LightAreaModificator::OVERALL_ILLUMINANCE_MODEL,
              modificatorIndex);
    }
  }
}

void ParticlesColorTechnique::adjustPipeline(mtt::GraphicsPipeline& pipeline)
{
  mtt::LogicalDevice& device = pipeline.device();

  std::unique_ptr<mtt::ShaderModule> vertexShader(
                                      new mtt::ShaderModule(
                                              mtt::ShaderModule::VERTEX_SHADER,
                                              device));
  vertexShader->newFragment().loadFromFile("particles.vert");
  _applyAreaModifictors(*vertexShader, pipeline);
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

  pipeline.setDefine("DEPTH_SAMPLER_ENABLED");
  pipeline.addResource( "depthSamplerBinding",
                        commonData().depthSampler,
                        VK_SHADER_STAGE_FRAGMENT_BIT);
}

void ParticlesColorTechnique::buildDrawAction(
                      mtt::DrawBin& renderBin,
                      mtt::DrawPlanBuildInfo& buildInfo,
                      mtt::GraphicsPipeline& pipeline,
                      uint32_t pointsNumber,
                      mtt::PlainBuffer& indices,
                      mtt::VolatileUniform<mtt::DrawMatrices>& matricesUniform)
{
  renderBin.createAction<ColorDrawParticlesAction>(
                              buildInfo.getPriorityFarFirstOrder(glm::vec3(0)),
                              pipeline,
                              buildInfo.viewport,
                              buildInfo.scissor,
                              pointsNumber,
                              indices,
                              matricesUniform,
                              buildInfo.drawMatrices,
                              *commonData().depthSamplerTexture);
}
