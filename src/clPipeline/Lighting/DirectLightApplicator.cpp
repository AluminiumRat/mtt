#include <limits>

#include <mtt/clPipeline/Lighting/DirectLight.h>
#include <mtt/clPipeline/Lighting/DirectLightApplicator.h>
#include <mtt/clPipeline/RenderPass/LightingPass.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/render/DrawPlan/DrawMeshAction.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/utilities/Abort.h>
#include <mtt/utilities/clip.h>

using namespace mtt;
using namespace mtt::clPipeline;

DirectLightApplicator::DrawTechnique::DrawTechnique(
                                            DirectLight& light,
                                            DirectLightApplicator& applicator) :
  _light(light),
  _applicator(applicator)
{
}

void DirectLightApplicator::DrawTechnique::invalidatePipeline() noexcept
{
  _pipeline.reset();
}

void DirectLightApplicator::DrawTechnique::_rebuildPipeline(
                                                AbstractRenderPass& renderPass)
{
  try
  {
    _pipeline.emplace(renderPass, lightingStage);
    _adjustPipeline();
    _pipeline->forceBuild();
  }
  catch(...)
  {
    invalidatePipeline();
    throw;
  }
}

void DirectLightApplicator::DrawTechnique::_adjustPipeline()
{
  _pipeline->addResource( "lightDataBinding",
                          _applicator._lightDataUniform,
                          VK_SHADER_STAGE_VERTEX_BIT |
                          VK_SHADER_STAGE_FRAGMENT_BIT);

  _pipeline->addResource( "depthMapBinding",
                          _applicator._depthMapSampler,
                          VK_SHADER_STAGE_FRAGMENT_BIT);

  _pipeline->addResource( "normalMapBinding",
                          _applicator._normalMapSampler,
                          VK_SHADER_STAGE_FRAGMENT_BIT);

  _pipeline->addResource( "albedoMapBinding",
                          _applicator._albedoMapSampler,
                          VK_SHADER_STAGE_FRAGMENT_BIT);

  _pipeline->addResource( "specularMapBinding",
                          _applicator._specularMapSampler,
                          VK_SHADER_STAGE_FRAGMENT_BIT);

  std::unique_ptr<ShaderModule> vertexShader(
                                  new ShaderModule( ShaderModule::VERTEX_SHADER,
                                                    _pipeline->device()));
  vertexShader->newFragment().loadFromFile(
                                        "clPipeline/directLightDrawable.vert");
  _pipeline->addShader(std::move(vertexShader));

  std::unique_ptr<ShaderModule> fragmentShader(
                                new ShaderModule( ShaderModule::FRAGMENT_SHADER,
                                                  _pipeline->device()));

  fragmentShader->newFragment().loadFromFile("clPipeline/materialLib.glsl");
  if(_light.shadowmapProvider() != nullptr)
  {
    _pipeline->setDefine("SHADOW_MAP_ENABLED");

    ShaderModule::Fragment& shadowLibFragment = fragmentShader->newFragment();
    shadowLibFragment.loadFromFile("clPipeline/cascadeShadowmapLib.glsl");

    shadowLibFragment.replace("$INDEX$", "");
    shadowLibFragment.replace("$SHADOW_CASCADE_SIZE$",
                              std::to_string(_light.cascadeSize()));

    _pipeline->addResource( "shadowMapBinding",
                            *_light.shadowmapSampler(),
                            VK_SHADER_STAGE_FRAGMENT_BIT);

    _pipeline->addResource( "shadowCoordsCorrectionBinding",
                            _applicator._coordsCorrectionUniform,
                            VK_SHADER_STAGE_FRAGMENT_BIT);
  }
  fragmentShader->newFragment().loadFromFile(
                                        "clPipeline/directLightDrawable.frag");
  _pipeline->addShader(std::move(fragmentShader));

  _pipeline->setTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP);
}

void DirectLightApplicator::DrawTechnique::addToDrawPlan(
                      DrawPlanBuildInfo& buildInfo,
                      const CascadeShadowMapProvider::CascadeInfo& cascadeInfo)
{
  AbstractRenderPass* renderPass = buildInfo.builder->stagePass(lightingStage);
  if(renderPass == nullptr) return;

  if (!_pipeline.has_value() || !_pipeline->isCompatible(*renderPass))
  {
    _rebuildPipeline(*renderPass);
  }

  DirectLightData lightData = _light.buildDrawData(buildInfo);

  if(cascadeInfo.empty()) _makeNonshadowCommand(buildInfo, lightData);
  else _makeShadowCommand( buildInfo, lightData, cascadeInfo);
}

void DirectLightApplicator::DrawTechnique::_makeNonshadowCommand(
                                              DrawPlanBuildInfo& buildInfo,
                                              const DirectLightData& lightData)
{
  DrawBin* renderBin = buildInfo.currentFramePlan->getBin(lightingStage);
  if(renderBin == nullptr) Abort("DirectLightApplicator::DrawTechnique::_makeNonshadowCommand: light render bin is not supported.");

  uint32_t pointsNumber = 4;

  using DrawAction = DrawMeshAction<VolatileUniform<DirectLightData>,
                                    DirectLightData,
                                    Texture2D,
                                    size_t,
                                    Texture2D,
                                    size_t,
                                    Texture2D,
                                    size_t,
                                    Texture2D,
                                    size_t>;
  renderBin->createAction<DrawAction>(0,
                                      *_pipeline,
                                      buildInfo.viewport,
                                      buildInfo.scissor,
                                      pointsNumber,
                                      _applicator._lightDataUniform,
                                      lightData,
                                      *_applicator._depthTexture,
                                      LightingPass::depthSamplerMapIndex,
                                      *_applicator._normalTexture,
                                      LightingPass::normalSamplerMapIndex,
                                      *_applicator._albedoTexture,
                                      LightingPass::albedoSamplerMapIndex,
                                      *_applicator._specularTexture,
                                      LightingPass::specularSamplerMapIndex);
}

void DirectLightApplicator::DrawTechnique::_makeShadowCommand(
                      DrawPlanBuildInfo& buildInfo,
                      const DirectLightData& lightData,
                      const CascadeShadowMapProvider::CascadeInfo& cascadeInfo)
{
  std::vector<Texture2D*> shadowTextures;
  shadowTextures.reserve(cascadeInfo.size());
  std::vector<ImageView*> shadowImageViews;
  shadowImageViews.reserve(cascadeInfo.size());
  CoordsCorrectionData correctionData;
  correctionData.reserve(cascadeInfo.size());

  Sampler& shadowmapSampler = *_light.shadowmapSampler();
  for(size_t layerIndex = 0; layerIndex < cascadeInfo.size(); layerIndex++)
  {
    float multiplicator = cascadeInfo[layerIndex].coordCorrection.multiplicator;
    float blurRadius = _light.blurRelativeRadius(multiplicator);

    correctionData.push_back(glm::vec4(
                          cascadeInfo[layerIndex].coordCorrection.multiplicator,
                          cascadeInfo[layerIndex].coordCorrection.shift.x,
                          cascadeInfo[layerIndex].coordCorrection.shift.y,
                          blurRadius));
    shadowTextures.push_back(
        static_cast<Texture2D*>(shadowmapSampler.attachedTexture(layerIndex)));
    shadowImageViews.push_back(cascadeInfo[layerIndex].map);
  }

  DrawBin* renderBin = buildInfo.currentFramePlan->getBin(lightingStage);
  if(renderBin == nullptr) Abort("DirectLightApplicator::DrawTechnique::_makeShadowCommand: light render bin is not supported.");

  uint32_t pointsNumber = 4;

  using DrawAction = DrawMeshAction<VolatileUniform<DirectLightData>,
                                    DirectLightData,
                                    VolatileUniform<CoordsCorrectionData>,
                                    CoordsCorrectionData,
                                    std::vector<Texture2D*>,
                                    std::vector<ImageView*>,
                                    Texture2D,
                                    size_t,
                                    Texture2D,
                                    size_t,
                                    Texture2D,
                                    size_t,
                                    Texture2D,
                                    size_t>;
  renderBin->createAction<DrawAction>(0,
                                      *_pipeline,
                                      buildInfo.viewport,
                                      buildInfo.scissor,
                                      pointsNumber,
                                      _applicator._lightDataUniform,
                                      lightData,
                                      _applicator._coordsCorrectionUniform,
                                      correctionData,
                                      shadowTextures,
                                      shadowImageViews,
                                      *_applicator._depthTexture,
                                      LightingPass::depthSamplerMapIndex,
                                      *_applicator._normalTexture,
                                      LightingPass::normalSamplerMapIndex,
                                      *_applicator._albedoTexture,
                                      LightingPass::albedoSamplerMapIndex,
                                      *_applicator._specularTexture,
                                      LightingPass::specularSamplerMapIndex);
}

DirectLightApplicator::DirectLightApplicator( DirectLight& light,
                                              LogicalDevice& device) :
  _device(device),
  _light(light),
  _depthMapSampler(PipelineResource::VOLATILE, device),
  _depthTexture(nullptr),
  _normalMapSampler(PipelineResource::VOLATILE, device),
  _normalTexture(nullptr),
  _albedoMapSampler(PipelineResource::VOLATILE, device),
  _albedoTexture(nullptr),
  _specularMapSampler(PipelineResource::VOLATILE, device),
  _specularTexture(nullptr),
  _technique(_light, *this)
{
  setLocalBoundSphere(Sphere( glm::vec3(0.f),
                              std::numeric_limits<float>::infinity()));

  std::shared_ptr<Texture2D> depthTexture = std::make_shared<Texture2D>(device);
  _depthTexture = depthTexture.get();
  _depthMapSampler.setAttachedTexture(std::move(depthTexture));

  std::shared_ptr<Texture2D> normalTexture =
                                            std::make_shared<Texture2D>(device);
  _normalTexture = normalTexture.get();
  _normalMapSampler.setAttachedTexture(std::move(normalTexture));

  std::shared_ptr<Texture2D> albedoTexture =
                                            std::make_shared<Texture2D>(device);
  _albedoTexture = albedoTexture.get();
  _albedoMapSampler.setAttachedTexture(std::move(albedoTexture));

  std::shared_ptr<Texture2D> specularTexture =
                                            std::make_shared<Texture2D>(device);
  _specularTexture = specularTexture.get();
  _specularMapSampler.setAttachedTexture(std::move(specularTexture));
}

void DirectLightApplicator::resetPipelines() noexcept
{
  _technique.invalidatePipeline();
}

void DirectLightApplicator::buildDrawActions(DrawPlanBuildInfo& buildInfo)
{
  if (buildInfo.frameType != colorFrameType) return;

  CascadeShadowMapProvider::CascadeInfo cascadeInfo;
  if(_light.shadowmapProvider() != nullptr)
  {
    CameraNode shadowmapCamera;
    CameraNode viewCamera;
    _light.updateCameras(shadowmapCamera, viewCamera, buildInfo);

    cascadeInfo = _light.shadowmapProvider()->getShadowMap(
                                                          shadowmapCamera,
                                                          viewCamera,
                                                          _light.cascadeSize(),
                                                          buildInfo);
    if (cascadeInfo.size() == 0) return;
  }

  _technique.addToDrawPlan(buildInfo, cascadeInfo);
}
