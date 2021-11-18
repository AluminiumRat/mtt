#include <mtt/DrawImpl/Lighting/DirectLightApplicator.h>
#include <mtt/DrawImpl/Lighting/ShadowMapProvider.h>
#include <mtt/DrawImpl/RenderPass/LightingPass.h>
#include <mtt/DrawImpl/constants.h>
#include <mtt/Render/DrawPlan/DrawMeshAction.h>
#include <mtt/Utilities/Abort.h>
#include <mtt/Utilities/clip.h>

using namespace mtt;

DirectLightApplicator::DrawTechnique::DrawTechnique(
                                                bool transparentRender,
                                                bool fullscreenRender,
                                                DirectLightApplicator& parent) :
  _transparentRender(transparentRender),
  _fullscreenRender(fullscreenRender),
  _parent(parent)
{
}

void DirectLightApplicator::DrawTechnique::invalidatePipeline() noexcept
{
  _pipeline.reset();
}

void DirectLightApplicator::DrawTechnique::_rebuildPipeline(
                                                AbstractRenderPass& renderPass,
                                                StageIndex stage)
{
  try
  {
    _pipeline.emplace(renderPass, stage);
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
                          _parent._lightDataUniform,
                          VK_SHADER_STAGE_VERTEX_BIT |
                          VK_SHADER_STAGE_FRAGMENT_BIT);

  _pipeline->addResource( "depthMapBinding",
                          _parent._depthMapSampler,
                          VK_SHADER_STAGE_FRAGMENT_BIT);

  _pipeline->addResource( "normalMapBinding",
                          _parent._normalMapSampler,
                          VK_SHADER_STAGE_FRAGMENT_BIT);

  _pipeline->addResource( "albedoMapBinding",
                          _parent._albedoMapSampler,
                          VK_SHADER_STAGE_FRAGMENT_BIT);

  _pipeline->addResource( "specularMapBinding",
                          _parent._specularMapSampler,
                          VK_SHADER_STAGE_FRAGMENT_BIT);

  if(_parent._shadowmapSampler.has_value())
  {
    _pipeline->setDefine("SHADOW_MAP_ENABLED");
    _pipeline->setDefine( "SHADOW_CASCADE_SIZE",
                          std::to_string(_parent.cascadeSize()));

    _pipeline->addResource( "shadowMapBinding",
                            _parent._shadowmapSampler.value(),
                            VK_SHADER_STAGE_FRAGMENT_BIT);

    _pipeline->addResource( "shadowCorrectionBinding",
                            _parent._shadowCorrectionUniform,
                            VK_SHADER_STAGE_FRAGMENT_BIT);

    _pipeline->addResource( "shadowCoordsCorrectionBinding",
                            _parent._coordsCorrectionUniform,
                            VK_SHADER_STAGE_FRAGMENT_BIT);
  }
  
  std::unique_ptr<ShaderModule> vertexShader(
                                  new ShaderModule( ShaderModule::VERTEX_SHADER,
                                                    _pipeline->device()));
  vertexShader->newFragment().loadFromFile("DrawImpl/directLight.vert");
  _pipeline->addShader(std::move(vertexShader));

  std::unique_ptr<ShaderModule> fragmentShader(
                                new ShaderModule( ShaderModule::FRAGMENT_SHADER,
                                                  _pipeline->device()));
  fragmentShader->newFragment().loadFromFile("DrawImpl/materialLib.frag");
  fragmentShader->newFragment().loadFromFile("DrawImpl/directLight.frag");
  _pipeline->addShader(std::move(fragmentShader));

  if(_fullscreenRender)
  {
    _pipeline->setTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP);
    _pipeline->setDefine("FULLSCREEN_ENABLED");
  }
  else
  {
    _pipeline->addResource( DrawMatrices::bindingName,
                            _parent._matricesUniform,
                            VK_SHADER_STAGE_VERTEX_BIT);
    _pipeline->setTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
  }

  if(_transparentRender) _pipeline->setDefine("ENABLE_DEPTH_PEELING");
}

void DirectLightApplicator::DrawTechnique::addToDrawPlan(
                                          DrawPlanBuildInfo& buildInfo,
                                          const CascadeInfo& cascadeInfo)
{
  StageIndex stage = _transparentRender ?  peelingLightingStage :
                                                    lightingStage;

  AbstractRenderPass* renderPass = buildInfo.builder->stagePass(stage);
  if(renderPass == nullptr) return;

  if (!_pipeline.has_value() || !_pipeline->isCompatible(*renderPass))
  {
    _rebuildPipeline(*renderPass, stage);
  }

  uint32_t pointsNumber = _fullscreenRender ? 4 : 36;

  LightData lightData;
  lightData.illuminance = _parent.baseIlluminance();
  lightData.lightInverseDirection =
              buildInfo.drawMatrices.localToViewMatrix * glm::vec4(0, 0, 1, 0);
  lightData.lightInverseDirection =
                                glm::normalize(lightData.lightInverseDirection);
  lightData.distance = _parent.distance();
  lightData.radius = _parent.radius();
  lightData.clipToView = buildInfo.drawMatrices.clipToViewMatrix;
  lightData.viewToLocal =
                        glm::inverse(buildInfo.drawMatrices.localToViewMatrix);

  if(cascadeInfo.empty())
  {
    _makeNonshadowCommand(buildInfo,
                          stage,
                          pointsNumber,
                          lightData);
  }
  else
  {
    _makeShadowCommand( buildInfo,
                        stage,
                        pointsNumber,
                        lightData,
                        cascadeInfo);
  }
}

void DirectLightApplicator::DrawTechnique::_makeNonshadowCommand(
                                                  DrawPlanBuildInfo& buildInfo,
                                                  StageIndex stage,
                                                  uint32_t pointsNumber,
                                                  const LightData& lightData)
{
  DrawBin* renderBin = buildInfo.currentFramePlan->getBin(stage);
  if(renderBin == nullptr) Abort("DirectLightApplicator::DrawTechnique::_makeNonshadowCommand: light render bin is not supported.");

  using DrawAction = DrawMeshAction<VolatileUniform<LightData>,
                                    LightData,
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
                                      _parent._lightDataUniform,
                                      lightData,
                                      _parent._matricesUniform,
                                      buildInfo.drawMatrices,
                                      *_parent._depthTexture,
                                      LightingPass::depthSamplerMapIndex,
                                      *_parent._normalTexture,
                                      LightingPass::normalSamplerMapIndex,
                                      *_parent._albedoTexture,
                                      LightingPass::albedoSamplerMapIndex,
                                      *_parent._specularTexture,
                                      LightingPass::specularSamplerMapIndex);
}

void DirectLightApplicator::DrawTechnique::_makeShadowCommand(
                                                DrawPlanBuildInfo& buildInfo,
                                                StageIndex stage,
                                                uint32_t pointsNumber,
                                                const LightData& lightData,
                                                const CascadeInfo& cascadeInfo)
{
  std::vector<Texture2D*> shadowTextures;
  shadowTextures.reserve(cascadeInfo.size());
  std::vector<ImageView*> shadowImageViews;
  shadowImageViews.reserve(cascadeInfo.size());
  CoordsCorrectionData correctionData;
  correctionData.reserve(cascadeInfo.size());
  for(size_t layerIndex = 0; layerIndex < cascadeInfo.size(); layerIndex++)
  {
    correctionData.push_back(cascadeInfo[layerIndex].coordCorrection);
    shadowTextures.push_back(_parent._shadowmapTextures[layerIndex].get());
    shadowImageViews.push_back(cascadeInfo[layerIndex].map);
  }

  DrawBin* renderBin = buildInfo.currentFramePlan->getBin(stage);
  if(renderBin == nullptr) Abort("DirectLightApplicator::DrawTechnique::_makeShadowCommand: light render bin is not supported.");

  using DrawAction = DrawMeshAction<VolatileUniform<LightData>,
                                    LightData,
                                    VolatileUniform<CoordsCorrectionData>,
                                    CoordsCorrectionData,
                                    VolatileUniform<ShadowCorrectionData>,
                                    ShadowCorrectionData,
                                    VolatileUniform<DrawMatrices>,
                                    DrawMatrices,
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
                                      _parent._lightDataUniform,
                                      lightData,
                                      _parent._coordsCorrectionUniform,
                                      correctionData,
                                      _parent._shadowCorrectionUniform,
                                      _parent._shadowCorrectionData,
                                      _parent._matricesUniform,
                                      buildInfo.drawMatrices,
                                      shadowTextures,
                                      shadowImageViews,
                                      *_parent._depthTexture,
                                      LightingPass::depthSamplerMapIndex,
                                      *_parent._normalTexture,
                                      LightingPass::normalSamplerMapIndex,
                                      *_parent._albedoTexture,
                                      LightingPass::albedoSamplerMapIndex,
                                      *_parent._specularTexture,
                                      LightingPass::specularSamplerMapIndex);
}

DirectLightApplicator::DirectLightApplicator(LogicalDevice& device) :
  _device(device),
  _shadowMapProvider(nullptr),
  _depthMapSampler(PipelineResource::VOLATILE, device),
  _depthTexture(nullptr),
  _normalMapSampler(PipelineResource::VOLATILE, device),
  _normalTexture(nullptr),
  _albedoMapSampler(PipelineResource::VOLATILE, device),
  _albedoTexture(nullptr),
  _specularMapSampler(PipelineResource::VOLATILE, device),
  _specularTexture(nullptr),
  _radius(10),
  _shadowCorrectionData{0, 1},
  _cascadeSize(3),
  _blurSize(0)
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

  _techniques.shapeSet.opaqueTechnique =
                                        std::make_unique<DrawTechnique>(false,
                                                                        false,
                                                                        *this);
  _techniques.shapeSet.transparentTechnique =
                                        std::make_unique<DrawTechnique>(true,
                                                                        false,
                                                                        *this);
  _techniques.fullscreenSet.opaqueTechnique =
                                        std::make_unique<DrawTechnique>(false,
                                                                        true,
                                                                        *this);
  _techniques.fullscreenSet.transparentTechnique =
                                        std::make_unique<DrawTechnique>(true,
                                                                        true,
                                                                        *this);
}

void DirectLightApplicator::setShadowMapProvider(
                                      ShadowMapProvider* newProvider) noexcept
{
  if(_shadowMapProvider == newProvider) return;

  _techniques.shapeSet.opaqueTechnique->invalidatePipeline();
  _techniques.shapeSet.transparentTechnique->invalidatePipeline();
  _techniques.fullscreenSet.opaqueTechnique->invalidatePipeline();
  _techniques.fullscreenSet.transparentTechnique->invalidatePipeline();

  _shadowmapSampler.reset();
  _shadowmapTextures.clear();

  _shadowMapProvider = newProvider;
}

void DirectLightApplicator::setCascadeSize(size_t newValue) noexcept
{
  if(_cascadeSize == newValue) return;
  if(newValue == 0) Abort("DirectLightApplicator::setCascadeSize: new value is 0");

  _techniques.shapeSet.opaqueTechnique->invalidatePipeline();
  _techniques.shapeSet.transparentTechnique->invalidatePipeline();
  _techniques.fullscreenSet.opaqueTechnique->invalidatePipeline();
  _techniques.fullscreenSet.transparentTechnique->invalidatePipeline();

  _shadowmapSampler.reset();
  _shadowmapTextures.clear();

  _cascadeSize = newValue;
}

void DirectLightApplicator::_rebuildShadowmapSampler()
{
  try
  {
    _shadowmapSampler.emplace(cascadeSize(),
                              PipelineResource::VOLATILE,
                              _device);

    for(size_t layerIndex = 0; layerIndex < cascadeSize(); layerIndex++)
    {
      _shadowmapTextures.push_back(std::make_shared<Texture2D>(_device));
      _shadowmapSampler->setAttachedTexture(_shadowmapTextures[layerIndex],
                                            layerIndex);
    }

    _shadowmapSampler->setMinFilter(VK_FILTER_LINEAR);
    _shadowmapSampler->setMagFilter(VK_FILTER_LINEAR);
    _shadowmapSampler->setMipmapMode(VK_SAMPLER_MIPMAP_MODE_NEAREST);
    _shadowmapSampler->setAddressModeU(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
    _shadowmapSampler->setAddressModeV(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
  }
  catch(...)
  {
    _shadowmapSampler.reset();
    _shadowmapTextures.clear();
    throw;
  }
}

bool DirectLightApplicator::_fullscreen(
                              const DrawPlanBuildInfo& buildInfo) const noexcept
{
  float halfdistance = distance() / 2;
  float boundRadius = sqrt(halfdistance * halfdistance + _radius * _radius);
  Sphere boundSphere(glm::vec3(0.f, 0.f, - halfdistance), boundRadius);
  boundSphere.translate(buildInfo.drawMatrices.localToViewMatrix);

  const Plane& nearPlane = buildInfo.viewFrustum.face(ViewFrustum::FACE_NEAR);

  return abs(nearPlane.signedDistance(boundSphere.center)) < boundSphere.radius;
}

void DirectLightApplicator::buildDrawActions(DrawPlanBuildInfo& buildInfo)
{
  if (buildInfo.frameType != colorFrameType) return;

  if(_shadowMapProvider != nullptr && !_shadowmapSampler.has_value())
  {
    _rebuildShadowmapSampler();
  }

  bool fullscreen = _fullscreen(buildInfo);
  TechniquesCollection::Set& set = fullscreen ? _techniques.fullscreenSet :
                                                _techniques.shapeSet;
  CascadeInfo cascadeInfo;
  if(_shadowMapProvider != nullptr)
  {
    cascadeInfo = _createShadowMap(buildInfo);
    if (cascadeInfo.size() == 0) return;
  }

  set.opaqueTechnique->addToDrawPlan(buildInfo, cascadeInfo);
  set.transparentTechnique->addToDrawPlan(buildInfo, cascadeInfo);
}

DirectLightApplicator::CascadeInfo
          DirectLightApplicator::_createShadowMap(DrawPlanBuildInfo& buildInfo)
{
  ShadowMapProvider::Area area = _getTopArea(buildInfo);
  if(area.size.x <= 0 || area.size.y <= 0) return CascadeInfo();

  glm::vec2 centralPoint = area.topleftCorner + area.size / 2.f;

  glm::vec2 directionPoint = _getCascadeDirectionPoint( buildInfo,
                                                        centralPoint);
  glm::vec2 shift = directionPoint - centralPoint;

  CascadeInfo result;
  result.reserve(_shadowmapTextures.size());

  for(size_t layerIndex = 0;
      layerIndex < _shadowmapTextures.size();
      layerIndex++)
  {
    ShadowMapProvider::Area alignedArea = _alignArea(area);
    float mapSize = float(_shadowMapProvider->frameExtent().x);
    float pixelSize = radius() * alignedArea.size.x / mapSize;
    float blurWidth = blurSize() / pixelSize;
    float blurLevel = blurWidth >= 1.f ? log2(blurWidth) : 0;

    ShadowMapInfo info{};
    info.map = &shadowMapProvider()->createShadowMap(
                                                    alignedArea,
                                                    blurLevel,
                                                    buildInfo.drawPlan,
                                                    *buildInfo.currentFramePlan,
                                                    buildInfo.rootViewInfo);
    glm::vec2 uvAreaSize = alignedArea.size / 2.f;
    glm::vec2 uvAreaCorner =
              alignedArea.topleftCorner * glm::vec2(.5f, .5f) + glm::vec2(.5f);

    info.coordCorrection = glm::vec4( 1.f / uvAreaSize.x,
                                      -uvAreaCorner.x / uvAreaSize.x,
                                      -uvAreaCorner.y / uvAreaSize.y,
                                      blurWidth / mapSize);
    result.push_back(info);

    shift /= 2.f;

    glm::vec2 center = area.topleftCorner + area.size / 2.f;
    center = center + shift;
    area.size /= 2.f;
    area.topleftCorner = center - area.size / 2.f;
  }

  return result;
}

ShadowMapProvider::Area DirectLightApplicator::_getTopArea(
                                    DrawPlanBuildInfo& buildInfo) const noexcept
{
  glm::mat4& clipToView = buildInfo.drawMatrices.clipToViewMatrix;
  glm::mat4 toLightView =
                        glm::inverse(buildInfo.drawMatrices.localToViewMatrix);
  glm::mat4 toLightClip = _shadowMapProvider->camera().projectionMatrix();
  glm::mat4 pointsTransform = toLightClip * toLightView * clipToView;

  glm::vec4 frustumPoints[] = { pointsTransform * glm::vec4(-1, -1, 1, 1),
                                pointsTransform * glm::vec4( 1, -1, 1, 1),
                                pointsTransform * glm::vec4( 1,  1, 1, 1),
                                pointsTransform * glm::vec4(-1,  1, 1, 1),
                                pointsTransform * glm::vec4(-1, -1, 0, 1),
                                pointsTransform * glm::vec4( 1, -1, 0, 1),
                                pointsTransform * glm::vec4( 1,  1, 0, 1),
                                pointsTransform * glm::vec4(-1,  1, 0, 1)};
  glm::vec4 clipArea{1,1,-1,-1};

  for(glm::vec4& point : frustumPoints)
  {
    if(point.x > point.w) point.x = point.w;
    if(point.x < -point.w) point.x = -point.w;
    point.x /= point.w;

    if(point.y > point.w) point.y = point.w;
    if(point.y < -point.w) point.y = -point.w;
    point.y /= point.w;

    clipArea.x = std::min(clipArea.x, point.x);
    clipArea.y = std::min(clipArea.y, point.y);
    clipArea.z = std::max(clipArea.z, point.x);
    clipArea.w = std::max(clipArea.w, point.y);
  }

  ShadowMapProvider::Area result;
  result.topleftCorner = glm::vec2(clipArea.x, clipArea.y);
  result.size = glm::vec2(clipArea.z - clipArea.x, clipArea.w - clipArea.y);
  return result;
}

glm::vec2 DirectLightApplicator::_getCascadeDirectionPoint(
                                          DrawPlanBuildInfo& buildInfo,
                                          glm::vec2 startPoint) const noexcept
{
  glm::mat4 toLightView =
                        glm::inverse(buildInfo.drawMatrices.localToViewMatrix);
  glm::mat4 toLightClip = _shadowMapProvider->camera().projectionMatrix();
  glm::mat4 endPointTransform = toLightClip * toLightView;
  glm::vec4 endPoint = endPointTransform *
                        glm::vec4(buildInfo.currentViewInfo.viewPosition, 1.f);
  glm::vec4 startPoint4d(startPoint, .5f, 1);
  clipSegment(startPoint4d, endPoint);
  glm::vec2 directionPoint = endPoint / endPoint.w;

  return directionPoint;
}

ShadowMapProvider::Area DirectLightApplicator::_alignArea(
                          const ShadowMapProvider::Area& source) const noexcept
{
  float areaSize = std::max(source.size.x, source.size.y);
  float sizeOrder = ceil(log2(areaSize));
  areaSize = exp2(sizeOrder);
  if(areaSize > 2.f) areaSize = 2.f;

  glm::vec2 mapExtent = _shadowMapProvider->frameExtent();
  float granularity = areaSize / mapExtent.x * 2.f;

  glm::vec2 sourceCenter = source.topleftCorner + source.size / 2.f;
  glm::vec2 alignedCorner = sourceCenter - glm::vec2(areaSize) / 2.f;
  alignedCorner = glm::round(alignedCorner / granularity) * granularity;
  alignedCorner = glm::clamp(alignedCorner, -1.f, 1.f);

  ShadowMapProvider::Area alignedArea;
  alignedArea.topleftCorner = alignedCorner;
  alignedArea.size = glm::vec2(areaSize);

  return alignedArea;
}
