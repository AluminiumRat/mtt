#include <mtt/clPipeline/Lighting/DirectLight.h>
#include <mtt/clPipeline/Lighting/DirectLightAreaModificator.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/DrawPlan/UpdateResourcesAction.h>
#include <mtt/render/Pipeline/AbstractPipeline.h>

using namespace mtt;
using namespace mtt::clPipeline;

DirectLightAreaModificator::DirectLightAreaModificator( DirectLight& light) :
  _light(light)
{
}

void DirectLightAreaModificator::updateBound() noexcept
{
  setLocalBoundSphere(_light.getBoundSphere());
}

void DirectLightAreaModificator::buildPrepareActions(
                                                  DrawPlanBuildInfo& buildInfo)
{
  if (buildInfo.frameType != colorFrameType) return;

  DirectLightDrawData lightData = _light.buildDrawData(buildInfo);

  ShadowMapProvider::CascadeInfo cascadeInfo;
  if(_light.shadowMapProvider() != nullptr)
  {
    cascadeInfo = _light.shadowMapProvider()->createShadowMap(
                                                          _light.cascadeSize(),
                                                          buildInfo);
    if (cascadeInfo.size() == 0) return;
  }

  if(cascadeInfo.empty())
  {
    _makeNonshadowCommand(buildInfo, lightData);
  }
  else
  {
    _makeShadowCommand(buildInfo, lightData, cascadeInfo);
  }
}

void DirectLightAreaModificator::_makeNonshadowCommand(
                                          DrawPlanBuildInfo& buildInfo,
                                          const DirectLightDrawData& lightData)
{
  DrawBin* renderBin =
                  buildInfo.currentFramePlan->getBin(modificatorsPrepareStage);
  if (renderBin == nullptr) Abort("DirectLightAreaModificator::_makeNonshadowCommand: prepare bin is not supported.");

  using Action = UpdateResourcesAction< VolatileUniform<DirectLightDrawData>,
                                        DirectLightDrawData>;
  renderBin->createAction<Action>(0, _lightDataUniform, lightData);
}

void DirectLightAreaModificator::_makeShadowCommand(
                              DrawPlanBuildInfo& buildInfo,
                              const DirectLightDrawData& lightData,
                              const ShadowMapProvider::CascadeInfo& cascadeInfo)
{
  std::vector<Texture2D*> shadowTextures;
  shadowTextures.reserve(cascadeInfo.size());
  std::vector<ImageView*> shadowImageViews;
  shadowImageViews.reserve(cascadeInfo.size());
  CoordsCorrectionData correctionData;
  correctionData.reserve(cascadeInfo.size());

  Sampler& shadowmapSampler = _light.getOrCreateShdowmapSampler();
  for(size_t layerIndex = 0; layerIndex < cascadeInfo.size(); layerIndex++)
  {
    float blurRadius = _light.blurRelativeRadius();
    blurRadius *= cascadeInfo[layerIndex].coordCorrection.multiplicator;

    correctionData.push_back(glm::vec4(
                          cascadeInfo[layerIndex].coordCorrection.multiplicator,
                          cascadeInfo[layerIndex].coordCorrection.shift.x,
                          cascadeInfo[layerIndex].coordCorrection.shift.y,
                          blurRadius));
    shadowTextures.push_back(
        static_cast<Texture2D*>(shadowmapSampler.attachedTexture(layerIndex)));
    shadowImageViews.push_back(cascadeInfo[layerIndex].map);
  }

  DrawBin* renderBin =
                  buildInfo.currentFramePlan->getBin(modificatorsPrepareStage);
  if (renderBin == nullptr) Abort("DirectLightAreaModificator::_makeShadowCommand: prepare bin is not supported.");

  using Action = UpdateResourcesAction< VolatileUniform<DirectLightDrawData>,
                                        DirectLightDrawData,
                                        VolatileUniform<CoordsCorrectionData>,
                                        CoordsCorrectionData,
                                        std::vector<Texture2D*>,
                                        std::vector<ImageView*>>;
  renderBin->createAction<Action>(0,
                                  _lightDataUniform,
                                  lightData,
                                  _coordsCorrectionUniform,
                                  correctionData,
                                  shadowTextures,
                                  shadowImageViews);
}

void DirectLightAreaModificator::adjustPipeline(
                                              AbstractPipeline& targetPipeline,
                                              ShaderModule& targetShader,
                                              size_t modificatorIndex)
{
  try
  {
    std::string indexStr = std::to_string(modificatorIndex);

    ShaderModule::Fragment& fragment = targetShader.newFragment();
    fragment.loadFromFile("clPipeline/directLightModificator.frag");

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

    const std::string* apply = targetShader.defineValue("APPLY_LIGHT");
    if (apply != nullptr)
    {
      std::string newApply = *apply + (applyFunctionName + "();");
      targetShader.setDefine("APPLY_LIGHT", newApply);
    }

    fragment.replace("$INDEX$", indexStr);

    std::string lightDataBindingName("lightDataBinding");
    lightDataBindingName += indexStr;
    targetPipeline.addResource( lightDataBindingName,
                                _lightDataUniform,
                                VK_SHADER_STAGE_FRAGMENT_BIT);

    if (_light.shadowMapProvider() != nullptr)
    {
      fragment.replace("$SHADOW_MAP_ENABLED$", "1");
      fragment.replace( "$SHADOW_CASCADE_SIZE$",
                        std::to_string(_light.cascadeSize()));

      std::string shadowSamplerBindingName("shadowMapBinding");
      shadowSamplerBindingName += indexStr;
      targetPipeline.addResource( shadowSamplerBindingName,
                                  _light.getOrCreateShdowmapSampler(),
                                  VK_SHADER_STAGE_FRAGMENT_BIT);

      std::string coordCorrectionBindingName("shadowCoordsCorrectionBinding");
      coordCorrectionBindingName += indexStr;
      targetPipeline.addResource(coordCorrectionBindingName,
                                  _coordsCorrectionUniform,
                                  VK_SHADER_STAGE_FRAGMENT_BIT);
    }
    else fragment.replace("$SHADOW_MAP_ENABLED$", "0");
  }
  catch (std::exception& error)
  {
    mtt::Log() << error.what();
    mtt::Abort("DirectLightAreaModificator::adjustPipeline: unable to adjust pipeline.");
  }
  catch (...)
  {
    mtt::Abort("DirectLightAreaModificator::adjustPipeline: unknown error.");
  }
}
