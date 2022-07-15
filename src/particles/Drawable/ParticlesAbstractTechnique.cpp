#include <mtt/particles/Drawable/ParticlesAbstractTechnique.h>
#include <mtt/render/RenderPass/GeneralRenderPass.h>

#define MIN_PARTICLES 10

using namespace mtt;

ParticlesAbstractTechnique::ParticlesAbstractTechnique(
                                      ParticlesDrawCommonData& commonData,
                                      StageIndex stage,
                                      uint8_t thinningFactor) :
  _commonData(commonData),
  _stage(stage),
  _thinningFactor(thinningFactor)
{
  if(_thinningFactor == 0) _thinningFactor = 1;
}

void ParticlesAbstractTechnique::clearPipeline() noexcept
{
  _pipeline.reset();
}

std::string ParticlesAbstractTechnique::_makeTextureExtentDefine() const
{
  std::string define;
  for ( size_t textureIndex = 0;
        textureIndex < _commonData.textureData.size();
        textureIndex++)
  {
    if(textureIndex != 0) define += ",";
    define += std::to_string(_commonData.textureData[textureIndex].extent);
  }
  return define;
}

void ParticlesAbstractTechnique::_rebuildPipeline(
                                                AbstractRenderPass& renderPass)
{
  _pipeline.emplace(renderPass, _stage);
  try
  {
    VertexAttribute& positionAtribute =
                            _pipeline->getOrCreateAttribute("positionLocation");
    positionAtribute.adjustDataType(VertexAttribute::FLOAT_VEC3_TYPE);
    positionAtribute.attachBuffer(&_commonData.positionBuffer);

    VertexAttribute& sizeRotationAtribute =
                        _pipeline->getOrCreateAttribute("sizeRotationLocation");
    sizeRotationAtribute.adjustDataType(VertexAttribute::FLOAT_VEC2_TYPE);
    sizeRotationAtribute.attachBuffer(&_commonData.sizeRotationBuffer);

    VertexAttribute& albedoAtribute =
                              _pipeline->getOrCreateAttribute("albedoLocation");
    albedoAtribute.adjustDataType(VertexAttribute::FLOAT_VEC4_TYPE);
    albedoAtribute.attachBuffer(&_commonData.albedoBuffer);

    VertexAttribute& emissionAtribute =
                            _pipeline->getOrCreateAttribute("emissionLocation");
    emissionAtribute.adjustDataType(VertexAttribute::FLOAT_VEC4_TYPE);
    emissionAtribute.attachBuffer(&_commonData.emissionBuffer);

    VertexAttribute& textureIndexAtribute =
                        _pipeline->getOrCreateAttribute("textureIndexLocation");
    textureIndexAtribute.adjustDataType(VertexAttribute::UINT32_TYPE);
    textureIndexAtribute.attachBuffer(&_commonData.textureIndexBuffer);

    VertexAttribute& tileIndexAtribute =
                          _pipeline->getOrCreateAttribute("tileIndexLocation");
    tileIndexAtribute.adjustDataType(VertexAttribute::UINT32_TYPE);
    tileIndexAtribute.attachBuffer(&_commonData.tileIndexBuffer);

    VertexAttribute& falloffFactorAtribute =
                    _pipeline->getOrCreateAttribute("falloffFactorLocation");
    falloffFactorAtribute.adjustDataType(VertexAttribute::FLOAT_TYPE);
    falloffFactorAtribute.attachBuffer(&_commonData.falloffFactorBuffer);

    _pipeline->addResource( DrawMatrices::bindingName,
                            _matricesUniform,
                            VK_SHADER_STAGE_VERTEX_BIT |
                              VK_SHADER_STAGE_GEOMETRY_BIT);

    _pipeline->addResource( "mppxFunctionBinding",
                            _mppxFunctionUniform,
                            VK_SHADER_STAGE_VERTEX_BIT);

    _pipeline->addResource( "falloffBinding",
                            _fallofUniform,
                            VK_SHADER_STAGE_VERTEX_BIT);

    if (_commonData.textureSampler.has_value())
    {
      _pipeline->addResource( "colorSamplerBinding",
                              _commonData.textureSampler.value(),
                              VK_SHADER_STAGE_FRAGMENT_BIT);
      _pipeline->setDefine("COLOR_SAMPLER_ENABLED");
      _pipeline->setDefine(
                      "TEXTURES_NUMBER",
                      std::to_string(_commonData.textureSampler->arraySize()));
      _pipeline->setDefine("EXTENT_DEFINE", _makeTextureExtentDefine());
    }

    if(_thinningFactor > 1.f)
    {
      _pipeline->setDefine("THINNING_FACTOR", std::to_string(_thinningFactor));
    }

    _pipeline->setTopology(VK_PRIMITIVE_TOPOLOGY_POINT_LIST);
    _pipeline->indices().setType(VK_INDEX_TYPE_UINT16);

    adjustPipeline(*_pipeline);
  }
  catch (...)
  {
    _pipeline.reset();
    throw;
  }
}

void ParticlesAbstractTechnique::addToDrawPlan(DrawPlanBuildInfo& buildInfo)
{
  AbstractRenderPass* renderPass = buildInfo.builder->stagePass(_stage);
  if (renderPass == nullptr) return;

  if (!_pipeline.has_value() || !_pipeline->isCompatible(*renderPass))
  {
    _rebuildPipeline(*renderPass);
  }

  DrawBin* renderBin = buildInfo.currentFramePlan->getBin(_stage);
  if (renderBin == nullptr) Abort("ParticlesAbstractTechnique::buildDrawActions: render bin is not supported.");

  IndicesData indices = _makeIndices(buildInfo, renderPass->device());
  if(indices.pointsNumber == 0) return;

  glm::vec2 falloffValue( _commonData.falloffBaseMppx,
                          _commonData.falloffBaseMppx *
                                          (1.f + _commonData.falloffSmoothing));

  buildDrawAction(*renderBin,
                  buildInfo,
                  *_pipeline,
                  uint32_t(indices.pointsNumber),
                  *indices.buffer,
                  _matricesUniform,
                  _mppxFunctionUniform,
                  _fallofUniform,
                  falloffValue);
}

ParticlesAbstractTechnique::IndicesData
                            ParticlesAbstractTechnique::_makeIndices(
                                            const DrawPlanBuildInfo& buildInfo,
                                            LogicalDevice& device) const
{
  float baseFallofFinishMppx =
            _commonData.falloffBaseMppx * (1.f + _commonData.falloffSmoothing);
  MppxDistanceFunction mppxFunction =
                                      buildInfo.currentViewInfo.mppxFunction();

  std::vector<float> distances;
  distances.reserve(_commonData.positionsData.size());
  std::vector<uint16_t> indicesData;
  indicesData.reserve(_commonData.positionsData.size());

  for(uint16_t particleIndex = 0;
      particleIndex < _commonData.particlesNumber;
      particleIndex++)
  {
    glm::vec3 particlePosition = _commonData.positionsData[particleIndex];
    float distance = buildInfo.distanceToPoint(particlePosition);

    distances.push_back(distance);

    float falloffFinishMppx =
            _commonData.falloffFactorData[particleIndex] * baseFallofFinishMppx;
    if(mppxFunction.mppx(distance) > falloffFinishMppx) continue;

    uint8_t tag = _commonData.tagData[particleIndex];
    if(tag % _thinningFactor != 0 && particleIndex > MIN_PARTICLES) continue;

    indicesData.push_back(particleIndex);
  }

  if (indicesData.size() == 0)
  {
    return IndicesData{ Ref<PlainBuffer>(), // buffer
                        0 };                // pointsNumber
  }

  std::sort(indicesData.begin(),
            indicesData.end(),
            [&](uint16_t firstIndex, uint16_t secondIndex) -> bool
            {
              return distances[firstIndex] > distances[secondIndex];
            });

  Ref<PlainBuffer> indexBuffer(new PlainBuffer(
                                          device,
                                          indicesData.size() * sizeof(uint16_t),
                                          VMA_MEMORY_USAGE_CPU_TO_GPU,
                                          VK_BUFFER_USAGE_INDEX_BUFFER_BIT));

  indexBuffer->uploadData(indicesData.data(),
                          0,
                          indicesData.size() * sizeof(uint16_t));

  return IndicesData{ indexBuffer,          // buffer
                      indicesData.size()};  // pointsNumber
}
