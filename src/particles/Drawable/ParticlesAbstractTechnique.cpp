#include <mtt/particles/Drawable/ParticlesAbstractTechnique.h>
#include <mtt/render/RenderPass/GeneralRenderPass.h>

#define MIN_PARTICLES 10

using namespace mtt;

ParticlesAbstractTechnique::ParticlesAbstractTechnique(
                                      ParticlesDrawCommonData& commonData,
                                      FrameType frameType,
                                      StageIndex stageIndex,
                                      uint8_t thinningFactor) :
  PipelineDrawable(frameType, stageIndex),
  _commonData(commonData),
  _thinningFactor(thinningFactor)
{
  if(_thinningFactor == 0) _thinningFactor = 1;
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

void ParticlesAbstractTechnique::adjustPipeline(GraphicsPipeline& pipeline)
{
  VertexAttribute& positionAtribute =
                              pipeline.getOrCreateAttribute("positionLocation");
  positionAtribute.adjustDataType(VertexAttribute::FLOAT_VEC3_TYPE);
  positionAtribute.attachBuffer(&_commonData.positionBuffer);

  VertexAttribute& sizeRotationAtribute =
                          pipeline.getOrCreateAttribute("sizeRotationLocation");
  sizeRotationAtribute.adjustDataType(VertexAttribute::FLOAT_VEC2_TYPE);
  sizeRotationAtribute.attachBuffer(&_commonData.sizeRotationBuffer);

  VertexAttribute& albedoAtribute =
                                pipeline.getOrCreateAttribute("albedoLocation");
  albedoAtribute.adjustDataType(VertexAttribute::FLOAT_VEC4_TYPE);
  albedoAtribute.attachBuffer(&_commonData.albedoBuffer);

  VertexAttribute& emissionAtribute =
                              pipeline.getOrCreateAttribute("emissionLocation");
  emissionAtribute.adjustDataType(VertexAttribute::FLOAT_VEC4_TYPE);
  emissionAtribute.attachBuffer(&_commonData.emissionBuffer);

  VertexAttribute& textureIndexAtribute =
                          pipeline.getOrCreateAttribute("textureIndexLocation");
  textureIndexAtribute.adjustDataType(VertexAttribute::UINT32_TYPE);
  textureIndexAtribute.attachBuffer(&_commonData.textureIndexBuffer);

  VertexAttribute& tileIndexAtribute =
                            pipeline.getOrCreateAttribute("tileIndexLocation");
  tileIndexAtribute.adjustDataType(VertexAttribute::UINT32_TYPE);
  tileIndexAtribute.attachBuffer(&_commonData.tileIndexBuffer);

  VertexAttribute& falloffFactorAtribute =
                        pipeline.getOrCreateAttribute("falloffFactorLocation");
  falloffFactorAtribute.adjustDataType(VertexAttribute::FLOAT_TYPE);
  falloffFactorAtribute.attachBuffer(&_commonData.falloffFactorBuffer);

  pipeline.addResource( DrawMatrices::bindingName,
                        _matricesUniform,
                        VK_SHADER_STAGE_VERTEX_BIT |
                          VK_SHADER_STAGE_GEOMETRY_BIT);

  pipeline.addResource( "mppxFunctionBinding",
                        _mppxFunctionUniform,
                        VK_SHADER_STAGE_VERTEX_BIT);

  pipeline.addResource( "falloffBinding",
                        _fallofUniform,
                        VK_SHADER_STAGE_VERTEX_BIT);

  if (_commonData.textureSampler.has_value())
  {
    pipeline.addResource( "colorSamplerBinding",
                          _commonData.textureSampler.value(),
                          VK_SHADER_STAGE_FRAGMENT_BIT);
    pipeline.setDefine("COLOR_SAMPLER_ENABLED");
    pipeline.setDefine(
                    "TEXTURES_NUMBER",
                    std::to_string(_commonData.textureSampler->arraySize()));
    pipeline.setDefine("EXTENT_DEFINE", _makeTextureExtentDefine());
  }

  if(_thinningFactor > 1.f)
  {
    pipeline.setDefine("THINNING_FACTOR", std::to_string(_thinningFactor));
  }

  pipeline.setTopology(VK_PRIMITIVE_TOPOLOGY_POINT_LIST);
  pipeline.indices().setType(VK_INDEX_TYPE_UINT16);
}

void ParticlesAbstractTechnique::buildDrawActions(DrawPlanBuildInfo& buildInfo)
{
  DrawBin* renderBin = buildInfo.currentFramePlan->getBin(stageIndex());
  if (renderBin == nullptr) Abort("ParticlesAbstractTechnique::buildDrawActions: render bin is not supported.");

  IndicesData indices = _makeIndices(buildInfo, pipeline()->device());
  if(indices.pointsNumber == 0) return;

  glm::vec2 falloffValue( _commonData.falloffBaseMppx,
                          _commonData.falloffBaseMppx *
                                          (1.f + _commonData.falloffSmoothing));

  buildDrawAction(*renderBin,
                  buildInfo,
                  *pipeline(),
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
