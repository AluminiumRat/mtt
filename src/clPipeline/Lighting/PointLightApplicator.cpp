#include <mtt/clPipeline/Lighting/CubeShadowmapProvider.h>
#include <mtt/clPipeline/Lighting/PointLight.h>
#include <mtt/clPipeline/Lighting/PointLightApplicator.h>
#include <mtt/clPipeline/RenderPass/LightingPass.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/render/DrawPlan/DrawMeshAction.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;
using namespace mtt::clPipeline;

PointLightApplicator::DrawTechnique::DrawTechnique(
                                            bool fullscreenRender,
                                            PointLight& light,
                                            PointLightApplicator& applicator) :
  PipelineDrawable(colorFrameType, lightingStage),
  _fullscreenRender(fullscreenRender),
  _light(light),
  _applicator(applicator)
{
}

void PointLightApplicator::DrawTechnique::adjustPipeline(
                                                    GraphicsPipeline& pipeline)
{
  pipeline.addResource( "lightDataBinding",
                        _applicator._lightDataUniform,
                        VK_SHADER_STAGE_VERTEX_BIT |
                        VK_SHADER_STAGE_FRAGMENT_BIT);

  pipeline.addResource( "depthMapBinding",
                        _applicator._depthMapSampler,
                        VK_SHADER_STAGE_FRAGMENT_BIT);

  pipeline.addResource( "normalMapBinding",
                        _applicator._normalMapSampler,
                        VK_SHADER_STAGE_FRAGMENT_BIT);

  pipeline.addResource( "albedoMapBinding",
                        _applicator._albedoMapSampler,
                        VK_SHADER_STAGE_FRAGMENT_BIT);

  pipeline.addResource( "specularMapBinding",
                        _applicator._specularMapSampler,
                        VK_SHADER_STAGE_FRAGMENT_BIT);

  std::unique_ptr<ShaderModule> vertexShader(
                                  new ShaderModule( ShaderModule::VERTEX_SHADER,
                                                    pipeline.device()));
  vertexShader->newFragment().loadFromFile(
                                          "clPipeline/pointLightDrawable.vert");
  pipeline.addShader(std::move(vertexShader));

  std::unique_ptr<ShaderModule> fragmentShader(
                                new ShaderModule( ShaderModule::FRAGMENT_SHADER,
                                                  pipeline.device()));

  fragmentShader->newFragment().loadFromFile("clPipeline/materialLib.glsl");

  ShaderModule::Fragment& fadeLibFragment = fragmentShader->newFragment();
  fadeLibFragment.loadFromFile("clPipeline/lightingFadeLib.glsl");
  fadeLibFragment.replace("$INDEX$", "");
  fadeLibFragment.replace("$FADE_TYPE$", std::to_string(_light.fadeType()));

  if(_light.shadowmapProvider() != nullptr)
  {
    pipeline.setDefine("SHADOW_MAP_ENABLED");

    ShaderModule::Fragment& shadowLibFragment = fragmentShader->newFragment();
    shadowLibFragment.loadFromFile("clPipeline/cubeShadowmapLib.glsl");

    shadowLibFragment.replace("$INDEX$", "");

    pipeline.addResource( "opaqueShadowMapBinding",
                          *_light.opaqueShadowmapSampler(),
                          VK_SHADER_STAGE_FRAGMENT_BIT);

    pipeline.addResource( "transparentShadowMapBinding",
                          *_light.transparentShadowmapSampler(),
                          VK_SHADER_STAGE_FRAGMENT_BIT);

    pipeline.addResource( "blurShiftsBinding",
                          *_light.blurShiftsBuffer(),
                          VK_SHADER_STAGE_FRAGMENT_BIT);
  }
  fragmentShader->newFragment().loadFromFile(
                                        "clPipeline/pointLightDrawable.frag");
  mtt::Sampler* filterSampler = _light.filterSampler();
  if (filterSampler != nullptr)
  {
    pipeline.addResource( "filterSamplerBinding",
                          *filterSampler,
                          VK_SHADER_STAGE_FRAGMENT_BIT);
    pipeline.setDefine("FILTER_SAMPLER_ENABLED");
  }
  pipeline.addShader(std::move(fragmentShader));

  if(_fullscreenRender)
  {
    pipeline.setTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP);
    pipeline.setDefine("FULLSCREEN_ENABLED");
  }
  else
  {
    pipeline.addResource( DrawMatrices::bindingName,
                          _applicator._matricesUniform,
                          VK_SHADER_STAGE_VERTEX_BIT);
    pipeline.setTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
  }
}

void PointLightApplicator::DrawTechnique::buildDrawActions(
                                                  DrawPlanBuildInfo& buildInfo)
{
  PointLightData lightData = _light.buildDrawData(buildInfo);
  uint32_t pointsNumber = _fullscreenRender ? 4 : 36;

  if(_light.shadowmapProvider() != nullptr)
  {
    CubeShadowmapProvider::Shadowmaps shadowmaps;
    shadowmaps = _light.shadowmapProvider()->getShadowMaps(
                                                  _light.shadowmapFrontCamera(),
                                                  buildInfo);
    if (shadowmaps.opaqueMap == nullptr ||
        shadowmaps.transparentMap == nullptr)
    {
      return;
    }

    _makeShadowCommand( buildInfo,
                        pointsNumber,
                        lightData,
                        *shadowmaps.opaqueMap,
                        *shadowmaps.transparentMap);
  }
  else _makeNonshadowCommand(buildInfo, pointsNumber, lightData);
}

void PointLightApplicator::DrawTechnique::_makeNonshadowCommand(
                                                DrawPlanBuildInfo& buildInfo,
                                                uint32_t pointsNumber,
                                                const PointLightData& lightData)
{
  DrawBin* renderBin = buildInfo.currentFramePlan->getBin(lightingStage);
  if(renderBin == nullptr) Abort("PointLightApplicator::DrawTechnique::_makeNonshadowCommand: light render bin is not supported.");

  using DrawAction = DrawMeshAction<VolatileUniform<PointLightData>,
                                    PointLightData,
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
                                      *pipeline(),
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

void PointLightApplicator::DrawTechnique::_makeShadowCommand(
                                                DrawPlanBuildInfo& buildInfo,
                                                uint32_t pointsNumber,
                                                const PointLightData& lightData,
                                                ImageView& opaqueShadowmap,
                                                ImageView& transparentShadowmap)
{
  Sampler& opaqueShadowmapSampler = *_light.opaqueShadowmapSampler();
  CubeTexture* opaqueShadowTexture =
          static_cast<CubeTexture*>(opaqueShadowmapSampler.attachedTexture(0));

  Sampler& transparentShadowmapSampler = *_light.transparentShadowmapSampler();
  CubeTexture* transparentShadowTexture =
      static_cast<CubeTexture*>(transparentShadowmapSampler.attachedTexture(0));

  DrawBin* renderBin = buildInfo.currentFramePlan->getBin(lightingStage);
  if(renderBin == nullptr) Abort("PointLightApplicator::DrawTechnique::_makeShadowCommand: light render bin is not supported.");

  using DrawAction = DrawMeshAction<VolatileUniform<PointLightData>,
                                    PointLightData,
                                    VolatileUniform<DrawMatrices>,
                                    DrawMatrices,
                                    CubeTexture,
                                    ImageView&,
                                    CubeTexture,
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
                                      *pipeline(),
                                      buildInfo.viewport,
                                      buildInfo.scissor,
                                      pointsNumber,
                                      _applicator._lightDataUniform,
                                      lightData,
                                      _applicator._matricesUniform,
                                      buildInfo.drawMatrices,
                                      *opaqueShadowTexture,
                                      opaqueShadowmap,
                                      *transparentShadowTexture,
                                      transparentShadowmap,
                                      *_applicator._depthTexture,
                                      LightingPass::depthSamplerMapIndex,
                                      *_applicator._normalTexture,
                                      LightingPass::normalSamplerMapIndex,
                                      *_applicator._albedoTexture,
                                      LightingPass::albedoSamplerMapIndex,
                                      *_applicator._specularTexture,
                                      LightingPass::specularSamplerMapIndex);
}

PointLightApplicator::PointLightApplicator( PointLight& light,
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

void PointLightApplicator::resetPipelines() noexcept
{
  _shapeTechnique.resetPipeline();
  _fullscreenTechnique.resetPipeline();
}

void PointLightApplicator::updateBound() noexcept
{
  setLocalBoundSphere(_light.getBoundSphere());
}

bool PointLightApplicator::_fullscreen(
                              const DrawPlanBuildInfo& buildInfo) const noexcept
{
  Sphere boundSphere = _light.getBoundSphere();
  boundSphere.translate(buildInfo.drawMatrices.localToViewMatrix);

  const Plane& nearPlane = buildInfo.viewFrustum.face(ViewFrustum::FACE_NEAR);

  return abs(nearPlane.signedDistance(boundSphere.center)) < boundSphere.radius;
}

void PointLightApplicator::buildDrawActions(DrawPlanBuildInfo& buildInfo)
{
  if (_light.distance() <= 0.f) return;

  if (_fullscreen(buildInfo)) _fullscreenTechnique.addToDrawPlan(buildInfo);
  else _shapeTechnique.addToDrawPlan(buildInfo);
}
