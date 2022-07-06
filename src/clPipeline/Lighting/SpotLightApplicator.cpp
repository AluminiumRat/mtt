#include <mtt/clPipeline/Lighting/SpotLight.h>
#include <mtt/clPipeline/Lighting/SpotLightApplicator.h>
#include <mtt/clPipeline/RenderPass/LightingPass.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/render/DrawPlan/DrawMeshAction.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;
using namespace mtt::clPipeline;

SpotLightApplicator::DrawTechnique::DrawTechnique(
                                              bool fullscreenRender,
                                              SpotLight& light,
                                              SpotLightApplicator& applicator) :
  _fullscreenRender(fullscreenRender),
  _light(light),
  _applicator(applicator)
{
}

void SpotLightApplicator::DrawTechnique::invalidatePipeline() noexcept
{
  _pipeline.reset();
}

void SpotLightApplicator::DrawTechnique::_rebuildPipeline(
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

void SpotLightApplicator::DrawTechnique::_adjustPipeline()
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
                                        "clPipeline/spotLightDrawable.vert");
  _pipeline->addShader(std::move(vertexShader));

  std::unique_ptr<ShaderModule> fragmentShader(
                                new ShaderModule( ShaderModule::FRAGMENT_SHADER,
                                                  _pipeline->device()));

  fragmentShader->newFragment().loadFromFile("clPipeline/materialLib.glsl");

  ShaderModule::Fragment& fadeLibFragment = fragmentShader->newFragment();
  fadeLibFragment.loadFromFile("clPipeline/lightingFadeLib.glsl");
  fadeLibFragment.replace("$INDEX$", "");
  fadeLibFragment.replace("$FADE_TYPE$", std::to_string(_light.fadeType()));

  if(_light.shadowMapProvider() != nullptr)
  {
    _pipeline->setDefine("SHADOW_MAP_ENABLED");

    ShaderModule::Fragment& shadowLibFragment = fragmentShader->newFragment();
    shadowLibFragment.loadFromFile("clPipeline/shadowmapLib.glsl");

    shadowLibFragment.replace("$INDEX$", "");

    _pipeline->addResource( "opaqueShadowMapBinding",
                            *_light.opaqueShadowmapSampler(),
                            VK_SHADER_STAGE_FRAGMENT_BIT);

    _pipeline->addResource( "transparentShadowMapBinding",
                            *_light.transparentShadowmapSampler(),
                            VK_SHADER_STAGE_FRAGMENT_BIT);
  }
  fragmentShader->newFragment().loadFromFile(
                                        "clPipeline/spotLightDrawable.frag");
  mtt::Sampler* filterSampler = _light.filterSampler();
  if (filterSampler != nullptr)
  {
    _pipeline->addResource( "filterSamplerBinding",
                            *filterSampler,
                            VK_SHADER_STAGE_FRAGMENT_BIT);
    _pipeline->setDefine("FILTER_SAMPLER_ENABLED");
  }
  _pipeline->addShader(std::move(fragmentShader));

  if(_fullscreenRender)
  {
    _pipeline->setTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP);
    _pipeline->setDefine("FULLSCREEN_ENABLED");
  }
  else
  {
    _pipeline->addResource( DrawMatrices::bindingName,
                            _applicator._matricesUniform,
                            VK_SHADER_STAGE_VERTEX_BIT);
    _pipeline->setTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
  }
}

void SpotLightApplicator::DrawTechnique::addToDrawPlan(
                                              DrawPlanBuildInfo& buildInfo,
                                              ImageView* opaqueShadowMap,
                                              ImageView* transparentShadowMap)
{
  AbstractRenderPass* renderPass = buildInfo.builder->stagePass(lightingStage);
  if(renderPass == nullptr) return;

  if (!_pipeline.has_value() || !_pipeline->isCompatible(*renderPass))
  {
    _rebuildPipeline(*renderPass);
  }

  uint32_t pointsNumber = _fullscreenRender ? 4 : 18;

  SpotLightData lightData = _light.buildDrawData(buildInfo);

  if(transparentShadowMap == nullptr || opaqueShadowMap == nullptr)
  {
    _makeNonshadowCommand(buildInfo, pointsNumber, lightData);
  }
  else
  {
    _makeShadowCommand( buildInfo,
                        pointsNumber,
                        lightData,
                        *opaqueShadowMap,
                        *transparentShadowMap);
  }
}

void SpotLightApplicator::DrawTechnique::_makeNonshadowCommand(
                                          DrawPlanBuildInfo& buildInfo,
                                          uint32_t pointsNumber,
                                          const SpotLightData& lightData)
{
  DrawBin* renderBin = buildInfo.currentFramePlan->getBin(lightingStage);
  if(renderBin == nullptr) Abort("SpotLightApplicator::DrawTechnique::_makeNonshadowCommand: light render bin is not supported.");

  using DrawAction = DrawMeshAction<VolatileUniform<SpotLightData>,
                                    SpotLightData,
                                    VolatileUniform<DrawMatrices>,
                                    DrawMatrices,
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
                                      _applicator._matricesUniform,
                                      buildInfo.drawMatrices,
                                      *_applicator._depthTexture,
                                      LightingPass::depthSamplerMapIndex,
                                      *_applicator._normalTexture,
                                      LightingPass::normalSamplerMapIndex,
                                      *_applicator._albedoTexture,
                                      LightingPass::albedoSamplerMapIndex,
                                      *_applicator._specularTexture,
                                      LightingPass::specularSamplerMapIndex);
}

void SpotLightApplicator::DrawTechnique::_makeShadowCommand(
                                                DrawPlanBuildInfo& buildInfo,
                                                uint32_t pointsNumber,
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

  DrawBin* renderBin = buildInfo.currentFramePlan->getBin(lightingStage);
  if(renderBin == nullptr) Abort("SpotLightApplicator::DrawTechnique::_makeShadowCommand: light render bin is not supported.");

  using DrawAction = DrawMeshAction<VolatileUniform<SpotLightData>,
                                    SpotLightData,
                                    VolatileUniform<DrawMatrices>,
                                    DrawMatrices,
                                    Texture2D,
                                    ImageView&,
                                    Texture2D,
                                    ImageView&,
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
                                      _applicator._matricesUniform,
                                      buildInfo.drawMatrices,
                                      *opaqueShadowTexture,
                                      opaqueShadowMap,
                                      *transparentShadowTexture,
                                      transparentShadowMap,
                                      *_applicator._depthTexture,
                                      LightingPass::depthSamplerMapIndex,
                                      *_applicator._normalTexture,
                                      LightingPass::normalSamplerMapIndex,
                                      *_applicator._albedoTexture,
                                      LightingPass::albedoSamplerMapIndex,
                                      *_applicator._specularTexture,
                                      LightingPass::specularSamplerMapIndex);
}

SpotLightApplicator::SpotLightApplicator( SpotLight& light,
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
  _shapeTechnique(false, _light, *this),
  _fullscreenTechnique(true, _light, *this)
{
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

void SpotLightApplicator::resetPipelines() noexcept
{
  _shapeTechnique.invalidatePipeline();
  _fullscreenTechnique.invalidatePipeline();
}

void SpotLightApplicator::updateBound() noexcept
{
  setLocalBoundSphere(_light.getBoundSphere());
}

bool SpotLightApplicator::_fullscreen(
                              const DrawPlanBuildInfo& buildInfo) const noexcept
{
  Sphere boundSphere = _light.getBoundSphere();
  boundSphere.translate(buildInfo.drawMatrices.localToViewMatrix);

  const Plane& nearPlane = buildInfo.viewFrustum.face(ViewFrustum::FACE_NEAR);

  return abs(nearPlane.signedDistance(boundSphere.center)) < boundSphere.radius;
}

void SpotLightApplicator::buildDrawActions(DrawPlanBuildInfo& buildInfo)
{
  if (buildInfo.frameType != colorFrameType) return;
  if (_light.angle() <= .0f) return;
  if (_light.distance() <= 0.f) return;

  ImageView* opaqueShadowmap = nullptr;
  ImageView* transparentShadowmap = nullptr;
  if(_light.shadowMapProvider() != nullptr)
  {
    ShadowMapProvider::Shadowmaps shadowmaps =
            _light.shadowMapProvider()->getShadowMaps(_light.shadowmapCamera(),
                                                      buildInfo);
    opaqueShadowmap = shadowmaps.opaqueMap;
    transparentShadowmap = shadowmaps.transparentMap;
  }

  if (_fullscreen(buildInfo))
  {
    _fullscreenTechnique.addToDrawPlan( buildInfo,
                                        opaqueShadowmap,
                                        transparentShadowmap);
  }
  else
  {
    _shapeTechnique.addToDrawPlan(buildInfo,
                                  opaqueShadowmap,
                                  transparentShadowmap);
  }
}
