#include <mtt/DLPipeline/Lighting/AmbientLightApplicator.h>
#include <mtt/DLPipeline/RenderPass/AmbientWeightPass.h>
#include <mtt/DLPipeline/RenderPass/LightingPass.h>
#include <mtt/DLPipeline/constants.h>
#include <mtt/Render/DrawPlan/DrawMeshAction.h>
#include <mtt/Utilities/Abort.h>

using namespace mtt;
using namespace mtt::DLPipeline;

AmbientLightApplicator::DrawTechnique::DrawTechnique(
                                              bool weightRender,
                                              bool transparentRender,
                                              bool fullScreenRender,
                                              AmbientLightApplicator& parent) :
  _weightRender(weightRender),
  _transparentRender(transparentRender),
  _fullScreenRender(fullScreenRender),
  _parent(parent)
{
}

void AmbientLightApplicator::DrawTechnique::invalidate() noexcept
{
  _pipeline.reset();
}

void AmbientLightApplicator::DrawTechnique::_rebuildPipeline(
                                                AbstractRenderPass& renderPass,
                                                StageIndex stage)
{
  try
  {
    _pipeline.emplace(renderPass, stage);
    _adjustPipeline(*_pipeline);
    _pipeline->forceBuild();
  }
  catch(...)
  {
    _pipeline.reset();
    throw;
  }
}

void AmbientLightApplicator::DrawTechnique::_adjustPipeline(
                                                    GraphicsPipeline & pipeline)
{
  pipeline.addResource( "lightDataBinding",
                        _parent._lightDataUniform,
                        VK_SHADER_STAGE_VERTEX_BIT |
                          VK_SHADER_STAGE_FRAGMENT_BIT);

  pipeline.addResource( "depthMapBinding",
                        _parent._depthMapSampler,
                        VK_SHADER_STAGE_FRAGMENT_BIT);

  if(!_weightRender)
  {
    pipeline.addResource( "weightMapBinding",
                          _parent._weightMapSampler,
                          VK_SHADER_STAGE_FRAGMENT_BIT);

    pipeline.addResource( "normalMapBinding",
                          _parent._normalMapSampler,
                          VK_SHADER_STAGE_FRAGMENT_BIT);

    pipeline.addResource( "albedoMapBinding",
                          _parent._albedoMapSampler,
                          VK_SHADER_STAGE_FRAGMENT_BIT);

    pipeline.addResource( "specularMapBinding",
                          _parent._specularMapSampler,
                          VK_SHADER_STAGE_FRAGMENT_BIT);

    if(_parent._ambientMapSampler.attachedTexture(0) != nullptr)
    {
      pipeline.addResource( "ambientMapBinding",
                            _parent._ambientMapSampler,
                            VK_SHADER_STAGE_FRAGMENT_BIT);
      pipeline.setDefine("AMBIENT_MAP_ENABLED");
    }

    if(_parent._luminanceMapSampler.attachedTexture(0) != nullptr)
    {
      pipeline.addResource( "diffuseLuminanceMapBinding",
                            _parent._luminanceMapSampler,
                            VK_SHADER_STAGE_FRAGMENT_BIT);
      pipeline.setDefine("DIFFUSE_LUMINANCE_MAP_ENABLED");
    }
  }

  std::unique_ptr<ShaderModule> vertexShader(
                                  new ShaderModule( ShaderModule::VERTEX_SHADER,
                                                    pipeline.device()));
  vertexShader->newFragment().loadFromFile("DLPipeline/ambientLight.vert");
  pipeline.addShader(std::move(vertexShader));

  std::unique_ptr<ShaderModule> fragmentShader(
                                new ShaderModule( ShaderModule::FRAGMENT_SHADER,
                                                  pipeline.device()));
  fragmentShader->newFragment().loadFromFile("DLPipeline/materialLib.frag");
  fragmentShader->newFragment().loadFromFile("DLPipeline/ambientLight.frag");
  pipeline.addShader(std::move(fragmentShader));

  if(_weightRender) pipeline.setDefine("WEIGHT_RENDER");

  if(_transparentRender) pipeline.setDefine("ENABLE_DEPTH_PEELING");

  if(_fullScreenRender)
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

  if(_parent._infinityAreaMode) _pipeline->setDefine("INFINITY_AREA");
}

void AmbientLightApplicator::DrawTechnique::addToDrawPlan(
                                                  DrawPlanBuildInfo& buildInfo)
{
  StageIndex stage(lightingStage);
  if(_transparentRender)
  {
    if (_weightRender) stage = peelingAmbientWeightStage;
    else stage = peelingLightingStage;
  }
  else
  {
    if(_weightRender) stage = ambientWeightStage;
    else stage = lightingStage;
  }
  AbstractRenderPass* renderPass = buildInfo.builder->stagePass(stage);
  if(renderPass == nullptr) return;

  if(!_pipeline.has_value() || !_pipeline->isCompatible(*renderPass))
  {
    _rebuildPipeline(*renderPass, stage);
  }

  uint32_t pointsNumber = _fullScreenRender ? 4 : 36;

  LightData lightData;
  lightData.illuminance = _parent.baseIlluminance();
  lightData.position =
              buildInfo.drawMatrices.localToViewMatrix * glm::vec4(0, 0, 0, 1);
  lightData.distance = _parent.distance();
  lightData.saturationDistance = std::min(_parent.saturationDistance(),
                                          _parent.distance());
  lightData.clipToView = buildInfo.drawMatrices.clipToViewMatrix;
  lightData.viewToLocal =
                        glm::inverse(buildInfo.drawMatrices.localToViewMatrix);

  DrawBin* renderBin = buildInfo.currentFramePlan->getBin(stage);
  if(renderBin == nullptr) Abort("AmbientLightApplicator::DrawTechnique::addToDrawPlan: light render bin is not supported.");

  if(_weightRender)
  {
    _makeWeightAction(buildInfo, *renderBin, pointsNumber, lightData);
  }
  else _makeApplyAction(buildInfo, *renderBin, pointsNumber, lightData);
}

void AmbientLightApplicator::DrawTechnique::_makeWeightAction(
                                                  DrawPlanBuildInfo& buildInfo,
                                                  DrawBin& renderBin,
                                                  uint32_t pointsNumber,
                                                  const LightData& lightData)
{
  using DrawAction = DrawMeshAction<VolatileUniform<LightData>,
                                    LightData,
                                    VolatileUniform<DrawMatrices>,
                                    DrawMatrices,
                                    Texture2D,
                                    size_t>;
  renderBin.createAction<DrawAction>( 
                          0,
                          *_pipeline,
                          buildInfo.viewport,
                          buildInfo.scissor,
                          pointsNumber,
                          _parent._lightDataUniform,
                          lightData,
                          _parent._matricesUniform,
                          buildInfo.drawMatrices,
                          *_parent._depthTexture,
                          AmbientWeightPass::depthSamplerMapIndex);
}

void AmbientLightApplicator::DrawTechnique::_makeApplyAction(
                                                  DrawPlanBuildInfo& buildInfo,
                                                  DrawBin& renderBin,
                                                  uint32_t pointsNumber,
                                                  const LightData& lightData)
{
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
                                    size_t,
                                    Texture2D,
                                    size_t>;
  renderBin.createAction<DrawAction>( 0,
                                      *_pipeline,
                                      buildInfo.viewport,
                                      buildInfo.scissor,
                                      pointsNumber,
                                      _parent._lightDataUniform,
                                      lightData,
                                      _parent._matricesUniform,
                                      buildInfo.drawMatrices,
                                      *_parent._weightTexture,
                                      LightingPass::weightSamplerMapIndex,
                                      *_parent._depthTexture,
                                      LightingPass::depthSamplerMapIndex,
                                      *_parent._normalTexture,
                                      LightingPass::normalSamplerMapIndex,
                                      *_parent._albedoTexture,
                                      LightingPass::albedoSamplerMapIndex,
                                      *_parent._specularTexture,
                                      LightingPass::specularSamplerMapIndex);
}

AmbientLightApplicator::AmbientLightApplicator(LogicalDevice& device) :
  _device(device),
  _infinityAreaMode(false),
  _saturationDistance(0.f),
  _weightMapSampler(PipelineResource::VOLATILE, device),
  _weightTexture(nullptr),
  _depthMapSampler(PipelineResource::VOLATILE, device),
  _depthTexture(nullptr),
  _normalMapSampler(PipelineResource::VOLATILE, device),
  _normalTexture(nullptr),
  _albedoMapSampler(PipelineResource::VOLATILE, device),
  _albedoTexture(nullptr),
  _specularMapSampler(PipelineResource::VOLATILE, device),
  _specularTexture(nullptr),
  _ambientMapSampler(PipelineResource::STATIC, device),
  _ambientMap(nullptr),
  _luminanceMapSampler(PipelineResource::STATIC, device),
  _diffuseLuminanceMap(nullptr)
{
  std::shared_ptr<Texture2D> weightTexture =
                                            std::make_shared<Texture2D>(device);
  _weightTexture = weightTexture.get();
  _weightMapSampler.setAttachedTexture(std::move(weightTexture));

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

  TechniquesCollection::Set& shapeSet = _techniques.shapeSet;
  shapeSet.weightTechnique =
                    std::make_unique<DrawTechnique>(true, false, false, *this);
  shapeSet.applyTechnique =
                    std::make_unique<DrawTechnique>(false, false, false, *this);
  shapeSet.peelingWeightTechnique =
                    std::make_unique<DrawTechnique>(true, true, false, *this);
  shapeSet.peelingApplyTechnique =
                    std::make_unique<DrawTechnique>(false, true, false, *this);
  
  TechniquesCollection::Set& fullscreenSet = _techniques.fullscreenSet;
  fullscreenSet.weightTechnique =
                    std::make_unique<DrawTechnique>(true, false, true, *this);
  fullscreenSet.applyTechnique =
                    std::make_unique<DrawTechnique>(false, false, true, *this);
  fullscreenSet.peelingWeightTechnique =
                    std::make_unique<DrawTechnique>(true, true, true, *this);
  fullscreenSet.peelingApplyTechnique =
                    std::make_unique<DrawTechnique>(false, true, true, *this);
}

void AmbientLightApplicator::_invalidateTechniques() noexcept
{
  _techniques.shapeSet.weightTechnique->invalidate();
  _techniques.shapeSet.applyTechnique->invalidate();
  _techniques.shapeSet.peelingWeightTechnique->invalidate();
  _techniques.shapeSet.peelingApplyTechnique->invalidate();

  _techniques.fullscreenSet.weightTechnique->invalidate();
  _techniques.fullscreenSet.applyTechnique->invalidate();
  _techniques.fullscreenSet.peelingWeightTechnique->invalidate();
  _techniques.fullscreenSet.peelingApplyTechnique->invalidate();
}

void AmbientLightApplicator::setAmbientMap(std::shared_ptr<CubeTexture> newMap)
{
  if(newMap.get() == _ambientMap) return;
  _ambientMap = newMap.get();
  _ambientMapSampler.setAttachedTexture(std::move(newMap), 0);
  _invalidateTechniques();
}

void AmbientLightApplicator::setDiffuseLuminanceMap(
                                            std::shared_ptr<CubeTexture> newMap)
{
  if(newMap.get() == _diffuseLuminanceMap) return;
  _diffuseLuminanceMap = newMap.get();
  _luminanceMapSampler.setAttachedTexture(std::move(newMap), 0);
  _invalidateTechniques();
}

bool AmbientLightApplicator::_fullscreen(
                              const DrawPlanBuildInfo& buildInfo) const noexcept
{
  if(_infinityAreaMode) return true;

  Sphere boundSphere(glm::vec3(0.f, 0.f, 0.f), distance() * sqrt(3.f));
  boundSphere.translate(buildInfo.drawMatrices.localToViewMatrix);

  const Plane& nearPlane = buildInfo.viewFrustum.face(ViewFrustum::FACE_NEAR);

  return abs(nearPlane.signedDistance(boundSphere.center)) < boundSphere.radius;
}

void AmbientLightApplicator::buildDrawActions(DrawPlanBuildInfo& buildInfo)
{
  if(buildInfo.frameType != colorFrameType) return;

  TechniquesCollection::Set& techniquesSet = _fullscreen(buildInfo) ?
                                                    _techniques.fullscreenSet :
                                                    _techniques.shapeSet;

  if(!_infinityAreaMode)
  {
    techniquesSet.weightTechnique->addToDrawPlan(buildInfo);
  }
  techniquesSet.applyTechnique->addToDrawPlan(buildInfo);

  if(!_infinityAreaMode)
  {
    techniquesSet.peelingWeightTechnique->addToDrawPlan(buildInfo);
  }
  techniquesSet.peelingApplyTechnique->addToDrawPlan(buildInfo);
}
