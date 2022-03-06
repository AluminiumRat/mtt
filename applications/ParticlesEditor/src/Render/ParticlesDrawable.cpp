#include <mtt/Application/Application.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/render/DrawPlan/DrawMeshAction.h>
#include <mtt/render/Pipeline/ShaderModule.h>
#include <mtt/render/RenderPass/GeneralRenderPass.h>
#include <mtt/utilities/Abort.h>

#include <Render/ParticlesDrawable.h>

ParticlesDrawable::DrawTechnique::DrawTechnique(ParticlesDrawable& parent) :
  _parent(parent)
{
}

void ParticlesDrawable::DrawTechnique::resetPipeline() noexcept
{
  _pipeline.reset();
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

    _pipeline->addResource( "positionsBufferBinding",
                            _parent._positionBuffer,
                            VK_SHADER_STAGE_VERTEX_BIT);

    _pipeline->addResource( "sizeRotationBufferBinding",
                            _parent._sizeRotationBuffer,
                            VK_SHADER_STAGE_VERTEX_BIT);

    _pipeline->addResource( "colorsBufferBinding",
                            _parent._colorBuffer,
                            VK_SHADER_STAGE_VERTEX_BIT);

    _pipeline->addResource( "textureIndicesBinding",
                            _parent._textureIndexBuffer,
                            VK_SHADER_STAGE_VERTEX_BIT);

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
    }

    _pipeline->setTopology(VK_PRIMITIVE_TOPOLOGY_POINT_LIST);
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

  uint32_t vertNumber = _parent._particlesNumber;

  using DrawAction = mtt::DrawMeshAction< MatricesUniform,
                                          mtt::DrawMatrices>;
  renderBin->createAction<DrawAction>(0,
                                      *_pipeline,
                                      buildInfo.viewport,
                                      buildInfo.scissor,
                                      vertNumber,
                                      _matricesUniform,
                                      buildInfo.drawMatrices);
}

ParticlesDrawable::ParticlesDrawable() :
  _positionBuffer(mtt::Application::instance().displayDevice(),
                  mtt::Buffer::UNIFORM_BUFFER),
  _sizeRotationBuffer(mtt::Application::instance().displayDevice(),
                      mtt::Buffer::UNIFORM_BUFFER),
  _colorBuffer( mtt::Application::instance().displayDevice(),
                mtt::Buffer::UNIFORM_BUFFER),
  _textureIndexBuffer(mtt::Application::instance().displayDevice(),
                      mtt::Buffer::UNIFORM_BUFFER),
  _particlesNumber(0),
  _colorTechnique(*this)
{
}

void ParticlesDrawable::setData(size_t particlesNumber,
                                glm::vec4* positionData,
                                glm::vec4* sizeRotationData,
                                glm::vec4* colorData,
                                uint32_t* textureIndexData)
{
  _particlesNumber = 0;
  if(particlesNumber == 0) return;

  _positionBuffer.setData(positionData, particlesNumber * sizeof(glm::vec4));
  _sizeRotationBuffer.setData(sizeRotationData,
                              particlesNumber * sizeof(glm::vec4));
  _colorBuffer.setData( colorData,
                        particlesNumber * sizeof(glm::vec4));
  _textureIndexBuffer.setData(textureIndexData,
                              particlesNumber * sizeof(uint32_t));

  _particlesNumber = particlesNumber;
}

void ParticlesDrawable::setParticleTextures(
                  const std::vector<std::shared_ptr<mtt::Texture2D>>& textures)
{
  _colorTechnique.resetPipeline();
  _sampler.reset();

  if(textures.empty()) return;

  try
  {
    _sampler.emplace( textures.size(),
                      mtt::PipelineResource::STATIC,
                      mtt::Application::instance().displayDevice());
    for ( size_t textureIndex = 0;
          textureIndex < textures.size();
          textureIndex++)
    {
      if(textures[textureIndex] == nullptr) mtt::Abort("ParticlesDrawable::setParticleTextures: textures array contain nullptr");
      _sampler->setAttachedTexture(textures[textureIndex], textureIndex);
    }
  }
  catch (...)
  {
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
