#include <mtt/clPipeline/Background/BackgroundAreaModificator.h>
#include <mtt/clPipeline/MeshTechniques/FLColorTechnique.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>

using namespace mtt;
using namespace mtt::clPipeline;

FLColorTechnique::FLColorTechnique( AreaModificatorSet& modificatorSet,
                                    VkPrimitiveTopology topology) :
  BaseGeometryTechnique(MATERIAL_FEATURE |
                          TEX_COORDS_FEATURE |
                          ALBEDO_SAMPLER_FEATURE |
                          OPAQUE_SAMPLER_FEATURE |
                          EMISSION_SAMPLER_FEATURE |
                          REFLECTION_SAMPLER_FEATURE |
                          VIEW_COORDS_IN_FRAGMENT_SHADER_FEATURE |
                          SKELETON_FEATURE,
                        forwardLightStage,
                        topology,
                        FAR_FIRST_ORDER),
  _modificators(modificatorSet)
{
  for (AreaModificator* modificator : _modificators.modificators())
  {
    if(modificator->type() == BackgroundAreaModificator::typeIndex)
    {
      modificator->addConsumer(*this);
    }
  }
}

FLColorTechnique::~FLColorTechnique()  noexcept
{
  for (AreaModificator* modificator : _modificators.modificators())
  {
    if (modificator->type() == BackgroundAreaModificator::typeIndex)
    {
      modificator->removeConsumer(*this);
    }
  }
}

void FLColorTechnique::resetPipeline() noexcept
{
  invalidatePipeline();
}

void FLColorTechnique::adjustPipeline(GraphicsPipeline& pipeline,
                                      AbstractRenderPass& renderPass)
{
  BaseGeometryTechnique::adjustPipeline(pipeline, renderPass);

  pipeline.setDepthTestEnable(true);
  pipeline.setDepthWriteEnable(true);
  pipeline.setDepthCompareOp(VK_COMPARE_OP_GREATER_OR_EQUAL);

  std::unique_ptr<ShaderModule> fragmentShader(
                                new ShaderModule( ShaderModule::FRAGMENT_SHADER,
                                                  renderPass.device()));
  fragmentShader->newFragment().loadFromFile("clPipeline/meshFL.frag");
  fragmentShader->setDefine("MODIFICATOR_DECLARATION", "");
  fragmentShader->setDefine("APPLY_POSTEFFECT", "");

  for(size_t modificatorIndex = 0;
      modificatorIndex < _modificators.modificators().size();
      modificatorIndex++)
  {
    AreaModificator& modificator =
                                *_modificators.modificators()[modificatorIndex];
    if(modificator.type() == BackgroundAreaModificator::typeIndex)
    {
      BackgroundAreaModificator& backgroundModificator =
                          static_cast<BackgroundAreaModificator&>(modificator);
      backgroundModificator.adjustPipeline( pipeline,
                                            *fragmentShader,
                                            modificatorIndex);
    }
  }

  pipeline.addShader(std::move(fragmentShader));
  pipeline.setDefine("ENABLE_ALPHA_TEST");
}
