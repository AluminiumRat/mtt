#include <limits>

#include <mtt/clPipeline/Lighting/DirectLight.h>
#include <mtt/clPipeline/Lighting/DirectLightAreaModificator.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/DrawPlan/UpdateResourcesAction.h>
#include <mtt/render/Pipeline/AbstractPipeline.h>
#include <mtt/render/SceneGraph/CameraNode.h>

using namespace mtt;
using namespace mtt::clPipeline;

DirectLightAreaModificator::DirectLightAreaModificator( DirectLight& light) :
  _light(light)
{
  setLocalBoundSphere(Sphere( glm::vec3(0.f),
                              std::numeric_limits<float>::infinity()));
}

void DirectLightAreaModificator::buildPrepareActions(
                                                  DrawPlanBuildInfo& buildInfo)
{
  if (buildInfo.frameType != colorFrameType) return;

  DirectLightData lightData = _light.buildDrawData(buildInfo);

  CascadeShadowMapProvider::CascadeInfo cascadeInfo;
  if(_light.shadowmapProvider() != nullptr)
  {
    CameraNode shadowmapCamera;
    CameraNode viewCamera;
    _light.updateCameras(shadowmapCamera, viewCamera, buildInfo);

    cascadeInfo = _light.shadowmapProvider()->getShadowMaps(
                                                          shadowmapCamera,
                                                          viewCamera,
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
                                              const DirectLightData& lightData)
{
  DrawBin* renderBin =
                  buildInfo.currentFramePlan->getBin(modificatorsPrepareStage);
  if (renderBin == nullptr) Abort("DirectLightAreaModificator::_makeNonshadowCommand: prepare bin is not supported.");

  using Action = UpdateResourcesAction< VolatileUniform<DirectLightData>,
                                        DirectLightData>;
  renderBin->createAction<Action>(0, _lightDataUniform, lightData);
}

void DirectLightAreaModificator::_makeShadowCommand(
                      DrawPlanBuildInfo& buildInfo,
                      const DirectLightData& lightData,
                      const CascadeShadowMapProvider::CascadeInfo& cascadeInfo)
{
  std::vector<Texture2D*> shadowTextures;
  shadowTextures.reserve(2 * cascadeInfo.size());
  std::vector<ImageView*> shadowImageViews;
  shadowImageViews.reserve(2 * cascadeInfo.size());
  CoordsCorrectionData correctionData;
  correctionData.reserve(cascadeInfo.size());

  Sampler& opaqueShadowmapSampler = *_light.opaqueShadowmapSampler();
  Sampler& transparentShadowmapSampler = *_light.transparentShadowmapSampler();

  for(size_t layerIndex = 0; layerIndex < cascadeInfo.size(); layerIndex++)
  {
    float multiplicator = cascadeInfo[layerIndex].coordCorrection.multiplicator;
    float blurRadius = _light.blurRelativeRadius(multiplicator);
    correctionData.push_back(glm::vec4(
                          cascadeInfo[layerIndex].coordCorrection.multiplicator,
                          cascadeInfo[layerIndex].coordCorrection.shift.x,
                          cascadeInfo[layerIndex].coordCorrection.shift.y,
                          blurRadius));

    shadowTextures.push_back(static_cast<Texture2D*>(
                          opaqueShadowmapSampler.attachedTexture(layerIndex)));
    shadowImageViews.push_back(cascadeInfo[layerIndex].maps.opaqueMap);

    shadowTextures.push_back(static_cast<Texture2D*>(
                      transparentShadowmapSampler.attachedTexture(layerIndex)));
    shadowImageViews.push_back(cascadeInfo[layerIndex].maps.transparentMap);
  }

  DrawBin* renderBin =
                  buildInfo.currentFramePlan->getBin(modificatorsPrepareStage);
  if (renderBin == nullptr) Abort("DirectLightAreaModificator::_makeShadowCommand: prepare bin is not supported.");

  using Action = UpdateResourcesAction< VolatileUniform<DirectLightData>,
                                        DirectLightData,
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
                                              ApplyModel applyModel,
                                              size_t modificatorIndex)
{
  try
  {
    std::string indexStr = std::to_string(modificatorIndex);

    if (_light.shadowmapProvider() != nullptr)
    {
      ShaderModule::Fragment& shadowLibFragment = targetShader.newFragment();
      shadowLibFragment.loadFromFile("clPipeline/cascadeShadowmapLib.glsl");

      shadowLibFragment.replace("$INDEX$", indexStr);

      shadowLibFragment.replace("$SHADOW_CASCADE_SIZE$",
                                std::to_string(_light.cascadeSize()));

      std::string shadowSamplerBindingName("opaqueShadowMapBinding");
      shadowSamplerBindingName += indexStr;
      targetPipeline.addResource( shadowSamplerBindingName,
                                  *_light.opaqueShadowmapSampler(),
                                  targetShader.type());

      shadowSamplerBindingName = ("transparentShadowMapBinding");
      shadowSamplerBindingName += indexStr;
      targetPipeline.addResource( shadowSamplerBindingName,
                                  *_light.transparentShadowmapSampler(),
                                  targetShader.type());

      std::string coordCorrectionBindingName("shadowCoordsCorrectionBinding");
      coordCorrectionBindingName += indexStr;
      targetPipeline.addResource(coordCorrectionBindingName,
                                  _coordsCorrectionUniform,
                                  targetShader.type());
    }

    ShaderModule::Fragment& mainFragment = targetShader.newFragment();
    mainFragment.loadFromFile("clPipeline/directLightModificator.glsl");

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
    mtt::Abort("DirectLightAreaModificator::adjustPipeline: unable to adjust pipeline.");
  }
  catch (...)
  {
    mtt::Abort("DirectLightAreaModificator::adjustPipeline: unknown error.");
  }
}
