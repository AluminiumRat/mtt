#include <mtt/clPipeline/Lighting/AmbientLightAreaModificator.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/DrawPlan/UpdateResourcesAction.h>
#include <mtt/render/Pipeline/AbstractPipeline.h>

using namespace mtt;
using namespace mtt::clPipeline;

const size_t AmbientLightAreaModificator::typeIndex =
                                                AreaModificator::allocateType();

AmbientLightAreaModificator::AmbientLightAreaModificator(
                                            const AmbientLightData& lightData,
                                            Sampler& ambientMapSampler,
                                            Sampler& diffuseLuminanceSampler) :
  AreaModificator(typeIndex),
  _lightData(lightData),
  _ambientMapSampler(ambientMapSampler),
  _diffuseLuminanceSampler(diffuseLuminanceSampler)
{
}

void AmbientLightAreaModificator::updateBound() noexcept
{
  setLocalBoundSphere(Sphere(glm::vec3(0.f), _lightData.distance));
}

void AmbientLightAreaModificator::buildPrepareActions(
                                                  DrawPlanBuildInfo& buildInfo)
{
  if (buildInfo.frameType != colorFrameType) return;
  DrawBin* renderBin =
                  buildInfo.currentFramePlan->getBin(modificatorsPrepareStage);
  if (renderBin == nullptr) Abort("AmbientLightAreaModificator::buildPrepareActions: prepare bin is not supported.");

  AmbientLightDrawData drawData;
  static_cast<AmbientLightData&>(drawData) = _lightData;
  drawData.position =
              buildInfo.drawMatrices.localToViewMatrix * glm::vec4(0, 0, 0, 1);
  drawData.clipToView = buildInfo.drawMatrices.clipToViewMatrix;
  drawData.viewToLocal = glm::inverse(buildInfo.drawMatrices.localToViewMatrix);

  using Action = UpdateResourcesAction< VolatileUniform<AmbientLightDrawData>,
                                        AmbientLightDrawData>;
  renderBin->createAction<Action>(0, _drawDataUniform, drawData);
}

void AmbientLightAreaModificator::adjustPipeline(
                                              AbstractPipeline& targetPipeline,
                                              ShaderModule& targetShader,
                                              size_t modificatorIndex)
{
  std::string indexStr = std::to_string(modificatorIndex);

  ShaderModule::Fragment& fragment = targetShader.newFragment();
  fragment.loadFromFile("clPipeline/ambientLightModificator.frag");

  std::string applyFunctionName("modificator");
  applyFunctionName += indexStr;
  fragment.replace("$APPLY_FUNCTION$", applyFunctionName);

  std::string weightFunction("ambientWeight");
  weightFunction += indexStr;
  fragment.replace("$WEIGHT_FUNCTION$", weightFunction);

  const std::string* declaration =
                            targetShader.defineValue("MODIFICATOR_DECLARATION");
  if (declaration != nullptr)
  {
    std::string newDeclaration = *declaration +
                            (std::string("void ") + applyFunctionName + "();");
    newDeclaration += std::string("void ") + weightFunction + "();";
    targetShader.setDefine("MODIFICATOR_DECLARATION", newDeclaration);
  }

  const std::string* applyWeight =
                              targetShader.defineValue("APPLY_AMBIENT_WEIGHT");
  if (applyWeight != nullptr)
  {
    std::string newApply = *applyWeight + (weightFunction + "();");
    targetShader.setDefine("APPLY_AMBIENT_WEIGHT", newApply);
  }

  const std::string* apply = targetShader.defineValue("APPLY_LIGHT");
  if (apply != nullptr)
  {
    std::string newApply = *apply + (applyFunctionName + "();");
    targetShader.setDefine("APPLY_LIGHT", newApply);
  }

  fragment.replace("$INDEX$", indexStr);

  if(_lightData.infinityAreaMode) fragment.replace("$INFINITY_AREA$", "1");
  else fragment.replace("$INFINITY_AREA$", "0");

  std::string lightDataBindingName("lightDataBinding");
  lightDataBindingName += indexStr;
  targetPipeline.addResource( lightDataBindingName,
                              _drawDataUniform,
                              VK_SHADER_STAGE_FRAGMENT_BIT);


  if(_ambientMapSampler.attachedTexture(0) != nullptr)
  {
    fragment.replace("$AMBIENT_MAP_ENABLED$", "1");

    std::string ambientMapBindingName("ambientMapBinding");
    ambientMapBindingName += indexStr;
    targetPipeline.addResource( ambientMapBindingName,
                                _ambientMapSampler,
                                VK_SHADER_STAGE_FRAGMENT_BIT);
  }
  else
  {
    fragment.replace("$AMBIENT_MAP_ENABLED$", "0");
  }

  if(_diffuseLuminanceSampler.attachedTexture(0) != nullptr)
  {
    fragment.replace("$DIFFUSE_LUMINANCE_MAP_ENABLED$", "1");

    std::string diffuseLuminanceMapBindingName("diffuseLuminanceMapBinding");
    diffuseLuminanceMapBindingName += indexStr;
    targetPipeline.addResource( diffuseLuminanceMapBindingName,
                                _diffuseLuminanceSampler,
                                VK_SHADER_STAGE_FRAGMENT_BIT);
  }
  else
  {
    fragment.replace("$DIFFUSE_LUMINANCE_MAP_ENABLED$", "0");
  }
}
