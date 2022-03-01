#include <mtt/Application/Application.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/render/DrawPlan/DrawMeshAction.h>
#include <mtt/render/Pipeline/ShaderModule.h>
#include <mtt/render/RenderPass/GeneralRenderPass.h>

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
    _pipeline->setDepthTestEnable(false);
    _pipeline->setDepthWriteEnable(false);
    _pipeline->setDepthCompareOp(VK_COMPARE_OP_GREATER_OR_EQUAL);

    std::unique_ptr<mtt::ShaderModule> vertexShader(
                                        new mtt::ShaderModule(
                                              mtt::ShaderModule::VERTEX_SHADER,
                                              renderPass.device()));
    vertexShader->newFragment().loadFromFile("particles.vert");
    _pipeline->addShader(std::move(vertexShader));

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
                            _parent._colorTransparencyBuffer,
                            VK_SHADER_STAGE_VERTEX_BIT);

    _pipeline->addResource( mtt::DrawMatrices::bindingName,
                            _matricesUniform,
                            VK_SHADER_STAGE_VERTEX_BIT);

    _pipeline->setTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
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

  uint32_t vertNumber = _parent._particlesNumber * 6;

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
  _colorTransparencyBuffer( mtt::Application::instance().displayDevice(),
                            mtt::Buffer::UNIFORM_BUFFER),
  _particlesNumber(0),
  _colorTechnique(*this)
{
}

void ParticlesDrawable::setData(size_t particlesNumber,
                                glm::vec4* positionData,
                                glm::vec4* sizeRotationData,
                                glm::vec4* colorTransparencyData)
{
  _particlesNumber = 0;
  if(particlesNumber == 0) return;

  _positionBuffer.setData(positionData, particlesNumber * sizeof(glm::vec4));
  _sizeRotationBuffer.setData(sizeRotationData,
                              particlesNumber * sizeof(glm::vec4));
  _colorTransparencyBuffer.setData( colorTransparencyData,
                                    particlesNumber * sizeof(glm::vec4));

  _particlesNumber = particlesNumber;
}

void ParticlesDrawable::buildDrawActions(mtt::DrawPlanBuildInfo& buildInfo)
{
  if(_particlesNumber == 0) return;
  if (buildInfo.frameType == mtt::clPipeline::colorFrameType)
  {
    _colorTechnique.buildDrawActions(buildInfo);
  }
}
