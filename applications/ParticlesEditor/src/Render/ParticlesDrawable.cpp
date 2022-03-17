#include <limits>

#include <mtt/Application/Application.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/render/DrawPlan/DrawMeshAction.h>
#include <mtt/render/Pipeline/ShaderModule.h>
#include <mtt/render/RenderPass/GeneralRenderPass.h>
#include <mtt/utilities/Abort.h>

#include <Render/DrawParticlesAction.h>
#include <Render/ParticlesDrawable.h>

ParticlesDrawable::DrawTechnique::DrawTechnique(ParticlesDrawable& parent) :
  _parent(parent)
{
}

void ParticlesDrawable::DrawTechnique::resetPipeline() noexcept
{
  _pipeline.reset();
}

std::string ParticlesDrawable::DrawTechnique::_makeTextureExtentefine() const
{
  std::string define;
  for ( size_t textureIndex = 0;
        textureIndex < _parent._textureData.size();
        textureIndex++)
  {
    if(textureIndex != 0) define += ",";
    define += std::to_string(_parent._textureData[textureIndex].extent);
  }
  return define;
}

void ParticlesDrawable::DrawTechnique::_rebuildPipeline(
                                            mtt::AbstractRenderPass& renderPass)
{
  _pipeline.emplace(renderPass, mtt::clPipeline::forwardLightStage);
  try
  {
    _pipeline->setDepthTestEnable(true);
    _pipeline->setDepthWriteEnable(false);
    _pipeline->setDepthCompareOp(VK_COMPARE_OP_GREATER_OR_EQUAL);

    std::unique_ptr<mtt::ShaderModule> vertexShader(
                                        new mtt::ShaderModule(
                                              mtt::ShaderModule::VERTEX_SHADER,
                                              renderPass.device()));
    vertexShader->newFragment().loadFromFile("particles.vert");
    _pipeline->addShader(std::move(vertexShader));

    std::unique_ptr<mtt::ShaderModule> geometryShader(
                                      new mtt::ShaderModule(
                                            mtt::ShaderModule::GEOMETRY_SHADER,
                                            renderPass.device()));
    geometryShader->newFragment().loadFromFile("particles.geom");
    _pipeline->addShader(std::move(geometryShader));

    std::unique_ptr<mtt::ShaderModule> fragmentShader(
                                      new mtt::ShaderModule(
                                            mtt::ShaderModule::FRAGMENT_SHADER,
                                            renderPass.device()));
    fragmentShader->newFragment().loadFromFile("particles.frag");
    _pipeline->addShader(std::move(fragmentShader));

    mtt::VertexAttribute& positionAtribute =
                            _pipeline->getOrCreateAttribute("positionLocation");
    positionAtribute.adjustDataType(mtt::VertexAttribute::FLOAT_VEC3_TYPE);
    positionAtribute.attachBuffer(&_parent._positionBuffer);

    mtt::VertexAttribute& sizeRotationAtribute =
                        _pipeline->getOrCreateAttribute("sizeRotationLocation");
    sizeRotationAtribute.adjustDataType(mtt::VertexAttribute::FLOAT_VEC2_TYPE);
    sizeRotationAtribute.attachBuffer(&_parent._sizeRotationBuffer);

    mtt::VertexAttribute& colorAtribute =
                              _pipeline->getOrCreateAttribute("colorLocation");
    colorAtribute.adjustDataType(mtt::VertexAttribute::FLOAT_VEC4_TYPE);
    colorAtribute.attachBuffer(&_parent._colorBuffer);

    mtt::VertexAttribute& textureIndexAtribute =
                        _pipeline->getOrCreateAttribute("textureIndexLocation");
    textureIndexAtribute.adjustDataType(mtt::VertexAttribute::UINT32_TYPE);
    textureIndexAtribute.attachBuffer(&_parent._textureIndexBuffer);

    mtt::VertexAttribute& tileIndexAtribute =
                          _pipeline->getOrCreateAttribute("tileIndexLocation");
    tileIndexAtribute.adjustDataType(mtt::VertexAttribute::UINT32_TYPE);
    tileIndexAtribute.attachBuffer(&_parent._tileIndexBuffer);

    _pipeline->addResource( mtt::DrawMatrices::bindingName,
                            _matricesUniform,
                            VK_SHADER_STAGE_VERTEX_BIT |
                              VK_SHADER_STAGE_GEOMETRY_BIT);

    if (_parent._sampler.has_value())
    {
      _pipeline->addResource( "colorSamplerBinding",
                              _parent._sampler.value(),
                              VK_SHADER_STAGE_FRAGMENT_BIT);
      _pipeline->setDefine("COLOR_SAMPLER_ENABLED");
      _pipeline->setDefine( "TEXTURES_NUMBER",
                            std::to_string(_parent._sampler->arraySize()));
      _pipeline->setDefine("EXTENT_DEFINE", _makeTextureExtentefine());
    }

    _pipeline->setTopology(VK_PRIMITIVE_TOPOLOGY_POINT_LIST);
    _pipeline->indices().setType(VK_INDEX_TYPE_UINT16);
  }
  catch (...)
  {
    _pipeline.reset();
    throw;
  }
}

void ParticlesDrawable::DrawTechnique::buildDrawActions(
                                              mtt::DrawPlanBuildInfo& buildInfo)
{
  mtt::AbstractRenderPass* renderPass =
            buildInfo.builder->stagePass(mtt::clPipeline::forwardLightStage);
  if (renderPass == nullptr) return;

  if (!_pipeline.has_value() || !_pipeline->isCompatible(*renderPass))
  {
    _rebuildPipeline(*renderPass);
  }

  mtt::DrawBin* renderBin =
        buildInfo.currentFramePlan->getBin(mtt::clPipeline::forwardLightStage);
  if (renderBin == nullptr) mtt::Abort("ParticlesDrawable::DrawTechnique::buildDrawActions: render bin is not supported.");

  mtt::Ref<mtt::PlainBuffer> indices =
                                  _makeIndices(buildInfo, renderPass->device());
  uint32_t pointsNumber = _parent._particlesNumber;

  renderBin->createAction<DrawParticleAction>(
                              buildInfo.getPriorityFarFirstOrder(glm::vec3(0)),
                              *_pipeline,
                              buildInfo.viewport,
                              buildInfo.scissor,
                              pointsNumber,
                              *indices,
                              _matricesUniform,
                              buildInfo.drawMatrices);
}

mtt::Ref<mtt::PlainBuffer> ParticlesDrawable::DrawTechnique::_makeIndices(
                                  const mtt::DrawPlanBuildInfo& buildInfo,
                                  mtt::LogicalDevice& device) const
{
  std::vector<float> distances;
  distances.reserve(_parent._positionsData.size());
  for (const glm::vec3& position : _parent._positionsData)
  {
    distances.push_back(buildInfo.distanceToPoint(position));
  }

  std::vector<uint16_t> indicesData;
  indicesData.reserve(_parent._positionsData.size());
  for(uint16_t i = 0; i < _parent._particlesNumber; i++)
  {
    indicesData.push_back(i);
  }

  std::sort(indicesData.begin(),
            indicesData.end(),
            [&](uint16_t firstIndex, uint16_t secondIndex) -> bool
            {
              return distances[firstIndex] > distances[secondIndex];
            });

  mtt::Ref<mtt::PlainBuffer> indexBuffer(new mtt::PlainBuffer(
                                          device,
                                          indicesData.size() * sizeof(uint16_t),
                                          VMA_MEMORY_USAGE_CPU_TO_GPU,
                                          VK_BUFFER_USAGE_INDEX_BUFFER_BIT));
  indexBuffer->uploadData(indicesData.data(),
                          0,
                          indicesData.size() * sizeof(uint16_t));
  return indexBuffer;
}

ParticlesDrawable::ParticlesDrawable() :
  _positionBuffer(mtt::Application::instance().displayDevice(),
                  mtt::Buffer::VERTEX_BUFFER),
  _sizeRotationBuffer(mtt::Application::instance().displayDevice(),
                      mtt::Buffer::VERTEX_BUFFER),
  _colorBuffer( mtt::Application::instance().displayDevice(),
                mtt::Buffer::VERTEX_BUFFER),
  _textureIndexBuffer(mtt::Application::instance().displayDevice(),
                      mtt::Buffer::VERTEX_BUFFER),
  _tileIndexBuffer( mtt::Application::instance().displayDevice(),
                    mtt::Buffer::VERTEX_BUFFER),
  _particlesNumber(0),
  _colorTechnique(*this)
{
}

void ParticlesDrawable::setData(std::vector<glm::vec3> positionData,
                                std::vector<glm::vec2> sizeRotationData,
                                std::vector<glm::vec4> colorData,
                                std::vector<uint32_t> textureIndexData,
                                std::vector<uint32_t> tileIndexData)
{
  if( positionData.size() != sizeRotationData.size() ||
      positionData.size() != colorData.size() ||
      positionData.size() != textureIndexData.size()) mtt::Abort("ParticlesDrawable::setData: data vectors have different sizes");

  _particlesNumber = 0;

  size_t particlesNumber = positionData.size();
  if(particlesNumber == 0) return;

  particlesNumber = std::min( particlesNumber,
                              size_t(std::numeric_limits<uint16_t>::max()));

  _positionBuffer.setData(positionData.data(),
                          particlesNumber * sizeof(glm::vec3));
  _sizeRotationBuffer.setData(sizeRotationData.data(),
                              particlesNumber * sizeof(glm::vec2));
  _colorBuffer.setData( colorData.data(),
                        particlesNumber * sizeof(glm::vec4));
  _textureIndexBuffer.setData(textureIndexData.data(),
                              particlesNumber * sizeof(uint32_t));
  _tileIndexBuffer.setData( tileIndexData.data(),
                            particlesNumber * sizeof(uint32_t));

  _positionsData = positionData;

  _particlesNumber = particlesNumber;
}

void ParticlesDrawable::setParticleTextures(
                                      const std::vector<TextureData>& textures)
{
  _colorTechnique.resetPipeline();
  _sampler.reset();
  _textureData.clear();

  if(textures.empty()) return;

  try
  {
    _textureData = textures;
    _sampler.emplace( _textureData.size(),
                      mtt::PipelineResource::STATIC,
                      mtt::Application::instance().displayDevice());
    for ( size_t textureIndex = 0;
          textureIndex < _textureData.size();
          textureIndex++)
    {
      if(_textureData[textureIndex].texture == nullptr) mtt::Abort("ParticlesDrawable::setParticleTextures: textures array contain nullptr");
      _sampler->setAttachedTexture( _textureData[textureIndex].texture,
                                    textureIndex);
    }
  }
  catch (...)
  {
    _textureData.clear();
    _sampler.reset();
    throw;
  }
}

void ParticlesDrawable::buildDrawActions(mtt::DrawPlanBuildInfo& buildInfo)
{
  if(_particlesNumber == 0) return;
  if (buildInfo.frameType == mtt::clPipeline::colorFrameType)
  {
    _colorTechnique.buildDrawActions(buildInfo);
  }
}
