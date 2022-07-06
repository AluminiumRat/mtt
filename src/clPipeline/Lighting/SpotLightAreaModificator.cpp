#include <mtt/clPipeline/Lighting/SpotLight.h>
#include <mtt/clPipeline/Lighting/SpotLightAreaModificator.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/DrawPlan/UpdateResourcesAction.h>
#include <mtt/render/Pipeline/AbstractPipeline.h>

using namespace mtt;
using namespace mtt::clPipeline;

SpotLightAreaModificator::SpotLightAreaModificator(SpotLight& light) :
  _light(light)
{
}

void SpotLightAreaModificator::updateBound() noexcept
{
  setLocalBoundSphere(_light.getBoundSphere());
}

void SpotLightAreaModificator::buildPrepareActions(
                                                  DrawPlanBuildInfo& buildInfo)
{
  if (buildInfo.frameType != colorFrameType) return;

  SpotLightData lightData = _light.buildDrawData(buildInfo);

  if(_light.shadowMapProvider() != nullptr)
  {
    ShadowMapProvider::Shadowmaps shadowmaps =
            _light.shadowMapProvider()->getShadowMaps(_light.shadowmapCamera(),
                                                      buildInfo);
    _makeShadowCommand( buildInfo,
                        lightData,
                        *shadowmaps.opaqueMap,
                        *shadowmaps.transparentMap);
  }
  else
  {
    _makeNonshadowCommand(buildInfo, lightData);
  }
}

void SpotLightAreaModificator::_makeNonshadowCommand(
                                                DrawPlanBuildInfo& buildInfo,
                                                const SpotLightData& lightData)
{
  DrawBin* renderBin =
                  buildInfo.currentFramePlan->getBin(modificatorsPrepareStage);
  if (renderBin == nullptr) Abort("SpotLightAreaModificator::_makeNonshadowCommand: prepare bin is not supported.");

  using Action = UpdateResourcesAction< VolatileUniform<SpotLightData>,
                                        SpotLightData>;
  renderBin->createAction<Action>(0, _lightDataUniform, lightData);
}

void SpotLightAreaModificator::_makeShadowCommand(
                                                DrawPlanBuildInfo& buildInfo,
                                                const SpotLightData& lightData,
                                                ImageView& opaqueShadowMap,
                                                ImageView& transparentShadowMap)
{
  Sampler& opaqueShadowmapSampler = *_light.opaqueShadowmapSampler();
  Texture2D* opaqueShadowTexture =
            static_cast<Texture2D*>(opaqueShadowmapSampler.attachedTexture(0));

  Sampler& transparentShadowmapSampler = *_light.transparentShadowmapSampler();
  Texture2D* transparentShadowTexture =
        static_cast<Texture2D*>(transparentShadowmapSampler.attachedTexture(0));

  DrawBin* renderBin =
                  buildInfo.currentFramePlan->getBin(modificatorsPrepareStage);
  if (renderBin == nullptr) Abort("SpotLightAreaModificator::_makeShadowCommand: prepare bin is not supported.");

  using Action = UpdateResourcesAction< VolatileUniform<SpotLightData>,
                                        SpotLightData,
                                        Texture2D,
                                        ImageView&,
                                        Texture2D,
                                        ImageView&>;
  renderBin->createAction<Action>(0,
                                  _lightDataUniform,
                                  lightData,
                                  *opaqueShadowTexture,
                                  opaqueShadowMap,
                                  *transparentShadowTexture,
                                  transparentShadowMap);
}

void SpotLightAreaModificator::adjustPipeline(
                                              AbstractPipeline& targetPipeline,
                                              ShaderModule& targetShader,
                                              ApplyModel applyModel,
                                              size_t modificatorIndex)
{
  try
  {
    std::string indexStr = std::to_string(modificatorIndex);

    ShaderModule::Fragment& fadeLibFragment = targetShader.newFragment();
    fadeLibFragment.loadFromFile("clPipeline/lightingFadeLib.glsl");
    fadeLibFragment.replace("$INDEX$", indexStr);
    fadeLibFragment.replace("$FADE_TYPE$", std::to_string(_light.fadeType()));

    if (_light.shadowMapProvider() != nullptr)
    {
      ShaderModule::Fragment& shadowLibFragment = targetShader.newFragment();
      shadowLibFragment.loadFromFile("clPipeline/shadowmapLib.glsl");

      shadowLibFragment.replace("$INDEX$", indexStr);

      std::string shadowSamplerBindingName("opaqueShadowMapBinding");
      shadowSamplerBindingName += indexStr;
      targetPipeline.addResource( shadowSamplerBindingName,
                                  *_light.opaqueShadowmapSampler(),
                                  targetShader.type());

      shadowSamplerBindingName = "transparentShadowMapBinding";
      shadowSamplerBindingName += indexStr;
      targetPipeline.addResource( shadowSamplerBindingName,
                                  *_light.transparentShadowmapSampler(),
                                  targetShader.type());
    }

    ShaderModule::Fragment& mainFragment = targetShader.newFragment();
    mainFragment.loadFromFile("clPipeline/spotLightModificator.glsl");

    std::string applyFunctionName("modificator");
    applyFunctionName += indexStr;
    mainFragment.replace("$APPLY_FUNCTION$", applyFunctionName);

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

    mainFragment.replace("$INDEX$", indexStr);

    std::string lightDataBindingName("lightDataBinding");
    lightDataBindingName += indexStr;
    targetPipeline.addResource( lightDataBindingName,
                                _lightDataUniform,
                                targetShader.type());

    mtt::Sampler* filterSampler = _light.filterSampler();
    if (filterSampler != nullptr)
    {
      std::string filterBindingName("filterSamplerBinding");
      filterBindingName += indexStr;
      targetPipeline.addResource( filterBindingName,
                                  *filterSampler,
                                  targetShader.type());
      mainFragment.replace("$FILTER_SAMPLER_ENABLED$", "1");
    }
    else mainFragment.replace("$FILTER_SAMPLER_ENABLED$", "0");

    if (_light.shadowMapProvider() != nullptr)
    {
      mainFragment.replace("$SHADOW_MAP_ENABLED$", "1");
    }
    else mainFragment.replace("$SHADOW_MAP_ENABLED$", "0");

    if (applyModel == LAMBERT_SPECULAR_LUMINANCE_MODEL)
    {
      mainFragment.replace("$LAMBERT_SPECULAR_LUMINANCE_MODEL$", "1");
    }
    else mainFragment.replace("$LAMBERT_SPECULAR_LUMINANCE_MODEL$", "0");
  }
  catch (std::exception& error)
  {
    mtt::Log() << error.what();
    mtt::Abort("SpotLightAreaModificator::adjustPipeline: unable to adjust pipeline.");
  }
  catch (...)
  {
    mtt::Abort("SpotLightAreaModificator::adjustPipeline: unknown error.");
  }
}
