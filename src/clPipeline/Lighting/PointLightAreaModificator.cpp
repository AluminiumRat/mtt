#include <mtt/clPipeline/Lighting/CubeShadowmapProvider.h>
#include <mtt/clPipeline/Lighting/PointLight.h>
#include <mtt/clPipeline/Lighting/PointLightAreaModificator.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/DrawPlan/UpdateResourcesAction.h>
#include <mtt/render/Pipeline/AbstractPipeline.h>

using namespace mtt;
using namespace mtt::clPipeline;

PointLightAreaModificator::PointLightAreaModificator(PointLight& light) :
  _light(light)
{
}

void PointLightAreaModificator::updateBound() noexcept
{
  setLocalBoundSphere(_light.getBoundSphere());
}

void PointLightAreaModificator::buildPrepareActions(
                                                  DrawPlanBuildInfo& buildInfo)
{
  if (buildInfo.frameType != colorFrameType) return;

  PointLightData lightData = _light.buildDrawData(buildInfo);

  ImageView* shadowMapView = nullptr;
  if(_light.shadowmapProvider() != nullptr)
  {
    ImageView& shadowMapView = _light.shadowmapProvider()->getShadowMap(
                                                  _light.shadowmapFrontCamera(),
                                                  buildInfo);
    _makeShadowCommand(buildInfo, lightData, shadowMapView);
  }
  else
  {
    _makeNonshadowCommand(buildInfo, lightData);
  }
}

void PointLightAreaModificator::_makeNonshadowCommand(
                                                DrawPlanBuildInfo& buildInfo,
                                                const PointLightData& lightData)
{
  DrawBin* renderBin =
                  buildInfo.currentFramePlan->getBin(modificatorsPrepareStage);
  if (renderBin == nullptr) Abort("PointLightAreaModificator::_makeNonshadowCommand: prepare bin is not supported.");

  using Action = UpdateResourcesAction< VolatileUniform<PointLightData>,
                                        PointLightData>;
  renderBin->createAction<Action>(0, _lightDataUniform, lightData);
}

void PointLightAreaModificator::_makeShadowCommand(
                                                DrawPlanBuildInfo& buildInfo,
                                                const PointLightData& lightData,
                                                ImageView& shadowMapView)
{
  Sampler& shadowmapSampler = *_light.shadowmapSampler();
  CubeTexture* shadowTexture =
                static_cast<CubeTexture*>(shadowmapSampler.attachedTexture(0));

  DrawBin* renderBin =
                  buildInfo.currentFramePlan->getBin(modificatorsPrepareStage);
  if (renderBin == nullptr) Abort("PointLightAreaModificator::_makeShadowCommand: prepare bin is not supported.");

  using Action = UpdateResourcesAction< VolatileUniform<PointLightData>,
                                        PointLightData,
                                        CubeTexture,
                                        ImageView&>;
  renderBin->createAction<Action>(0,
                                  _lightDataUniform,
                                  lightData,
                                  *shadowTexture,
                                  shadowMapView);
}

void PointLightAreaModificator::adjustPipeline(
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

    if (_light.shadowmapProvider() != nullptr)
    {
      ShaderModule::Fragment& shadowLibFragment = targetShader.newFragment();
      shadowLibFragment.loadFromFile("clPipeline/cubeShadowmapLib.glsl");

      shadowLibFragment.replace("$INDEX$", indexStr);

      std::string shadowSamplerBindingName("shadowMapBinding");
      shadowSamplerBindingName += indexStr;
      targetPipeline.addResource( shadowSamplerBindingName,
                                  *_light.shadowmapSampler(),
                                  targetShader.type());

      std::string blurShiftsBindingName("blurShiftsBinding");
      blurShiftsBindingName += indexStr;
      targetPipeline.addResource( blurShiftsBindingName,
                                  *_light.blurShiftsBuffer(),
                                  VK_SHADER_STAGE_FRAGMENT_BIT);
    }

    ShaderModule::Fragment& mainFragment = targetShader.newFragment();
    mainFragment.loadFromFile("clPipeline/pointLightModificator.glsl");

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
                                  VK_SHADER_STAGE_FRAGMENT_BIT);
      mainFragment.replace("$FILTER_SAMPLER_ENABLED$", "1");
    }
    else mainFragment.replace("$FILTER_SAMPLER_ENABLED$", "0");

    if (_light.shadowmapProvider() != nullptr)
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
    mtt::Abort("PointLightAreaModificator::adjustPipeline: unable to adjust pipeline.");
  }
  catch (...)
  {
    mtt::Abort("PointLightAreaModificator::adjustPipeline: unknown error.");
  }
}
