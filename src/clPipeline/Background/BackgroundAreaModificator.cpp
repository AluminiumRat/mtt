#include <stdexcept>

#include <mtt/clPipeline/Background/BackgroundAreaModificator.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/DrawPlan/UpdateResourcesAction.h>
#include <mtt/render/Pipeline/AbstractPipeline.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;
using namespace mtt::clPipeline;

const size_t BackgroundAreaModificator::typeIndex =
                                                AreaModificator::allocateType();

BackgroundAreaModificator::BackgroundAreaModificator(
                                        const BackgroundProperties& properties,
                                        Sampler& luminanceSampler) :
  AreaModificator(typeIndex),
  _properties(properties),
  _luminanceSampler(luminanceSampler)
{
}

void BackgroundAreaModificator::buildPrepareActions(
                                                  DrawPlanBuildInfo& buildInfo)
{
  if (buildInfo.frameType != colorFrameType) return;

  DrawBin* renderBin =
                  buildInfo.currentFramePlan->getBin(modificatorsPrepareStage);
  if (renderBin == nullptr) Abort("BackgroundAreaModificator::buildPrepareActions: prepare bin is not supported.");

  BackgroundDrawData drawData;
  drawData.properties = _properties;
  drawData.viewToLocal = glm::inverse(buildInfo.drawMatrices.localToViewMatrix);

  using Action = UpdateResourcesAction< VolatileUniform<BackgroundDrawData>,
                                        BackgroundDrawData>;
  renderBin->createAction<Action>(0, _drawDataUniform, drawData);
}

void BackgroundAreaModificator::adjustPipeline(
                                              AbstractPipeline& targetPipeline,
                                              ShaderModule& targetShader,
                                              size_t modificatorIndex)
{
  std::string indexStr = std::to_string(modificatorIndex);

  ShaderModule::Fragment& fragment = targetShader.newFragment();
  fragment.loadFromFile("clPipeline/backgroundModificator.glsl");

  std::string applyFunctionName("modificator");
  applyFunctionName += indexStr;
  fragment.replace("$APPLY_FUNCTION$", applyFunctionName);

  const std::string* declaration =
                            targetShader.defineValue("MODIFICATOR_DECLARATION");
  if (declaration != nullptr)
  {
    std::string newDeclaration = *declaration +
                            (std::string("void ") + applyFunctionName + "();");
    targetShader.setDefine("MODIFICATOR_DECLARATION", newDeclaration);
  }

  const std::string* apply = targetShader.defineValue("APPLY_POSTEFFECT");
  if (apply != nullptr)
  {
    std::string newApply = *apply + (applyFunctionName + "();");
    targetShader.setDefine("APPLY_POSTEFFECT", newApply);
  }

  fragment.replace("$INDEX$", indexStr);

  std::string drawDataBindingName("drawDataBinding");
  drawDataBindingName += indexStr;
  targetPipeline.addResource( drawDataBindingName,
                              _drawDataUniform,
                              targetShader.type());

  if(_luminanceSampler.attachedTexture(0) != nullptr)
  {
    fragment.replace("$LUMINANCE_SAMPLER_ENABLED$", "1");

    std::string luminanceSamplerBindingName("luminanceTextureBinding");
    luminanceSamplerBindingName += indexStr;
    targetPipeline.addResource( luminanceSamplerBindingName,
                                _luminanceSampler,
                                targetShader.type());
  }
  else
  {
    fragment.replace("$LUMINANCE_SAMPLER_ENABLED$", "0");
  }
}
