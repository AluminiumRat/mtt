#include <mtt/clPipeline/RenderPass/BackgroundPass.h>
#include <mtt/clPipeline/BackgroundDrawable.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/render/DrawPlan/DrawMeshAction.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;
using namespace mtt::clPipeline;

BackgroundDrawable::DrawTechnique::DrawTechnique(BackgroundDrawable& parent) :
  _parent(parent)
{
}

void BackgroundDrawable::DrawTechnique::invalidate() noexcept
{
  _pipeline.reset();
}

void BackgroundDrawable::DrawTechnique::_rebuildPipeline(
                                                AbstractRenderPass& renderPass,
                                                StageIndex stage)
{
  try
  {
    _pipeline.emplace(renderPass, stage);

    _pipeline->addResource( "drawDataBinding",
                            _parent._drawDataUniform,
                            VK_SHADER_STAGE_FRAGMENT_BIT);

    _pipeline->addResource( "drawMatricesBinding",
                            _parent._matricesUniform,
                            VK_SHADER_STAGE_FRAGMENT_BIT);

    _pipeline->addResource( "depthMapBinding",
                            _parent._depthAttachment,
                            VK_SHADER_STAGE_FRAGMENT_BIT);

    if(_parent._luminanceSampler.attachedTexture(0) != nullptr)
    {
      _pipeline->addResource( "luminanceTextureBinding",
                              _parent._luminanceSampler,
                              VK_SHADER_STAGE_FRAGMENT_BIT);
      _pipeline->setDefine("LUMINANCE_SAMPLER_ENABLED");
    }

    std::unique_ptr<ShaderModule> vertexShader(
                                  new ShaderModule( ShaderModule::VERTEX_SHADER,
                                                    _pipeline->device()));
    vertexShader->newFragment().loadFromFile("clPipeline/background.vert");
    _pipeline->addShader(std::move(vertexShader));

    std::unique_ptr<ShaderModule> fragmentShader(
                                new ShaderModule( ShaderModule::FRAGMENT_SHADER,
                                                  _pipeline->device()));
    fragmentShader->newFragment().loadFromFile("clPipeline/background.frag");
    _pipeline->addShader(std::move(fragmentShader));

    _pipeline->setTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP);

    _pipeline->forceBuild();
  }
  catch(...)
  {
    invalidate();
    throw;
  }
}

void BackgroundDrawable::DrawTechnique::addToDrawPlan(
                                                  DrawPlanBuildInfo& buildInfo)
{
  StageIndex stage =  backgroundStage;
  AbstractRenderPass* renderPass = buildInfo.builder->stagePass(stage);
  if(renderPass == nullptr) return;

  if (!_pipeline.has_value() || !_pipeline->isCompatible(*renderPass))
  {
    _rebuildPipeline(*renderPass, stage);
  }

  DrawBin* renderBin = buildInfo.currentFramePlan->getBin(stage);
  if(renderBin == nullptr) Abort("BackgroundDrawable::DrawTechnique: background render bin is not supported.");

  uint32_t pointsNumber = 4;

  DrawData drawData;
  drawData.properties = _parent._properties;
  drawData.viewToLocal = glm::inverse(buildInfo.drawMatrices.localToViewMatrix);

  using DrawAction = DrawMeshAction<VolatileUniform<DrawData>,
                                    DrawData,
                                    VolatileUniform<DrawMatrices>,
                                    DrawMatrices,
                                    InputAttachment,
                                    uint32_t>;
  renderBin->createAction<DrawAction>(0,
                                      *_pipeline,
                                      buildInfo.viewport,
                                      buildInfo.scissor,
                                      pointsNumber,
                                      _parent._drawDataUniform,
                                      drawData,
                                      _parent._matricesUniform,
                                      buildInfo.drawMatrices,
                                      _parent._depthAttachment,
                                      BackgroundPass::depthMapAttachmentIndex);
}

BackgroundDrawable::BackgroundDrawable(LogicalDevice& device) :
  _device(device),
  _properties{glm::vec3(1.f),
              40.f,
              10.f},
  _depthAttachment(device),
  _luminanceSampler(PipelineResource::STATIC, device),
  _technique(*this)
{
}

void BackgroundDrawable::buildDrawActions(DrawPlanBuildInfo& buildInfo)
{
  if(buildInfo.frameType != colorFrameType) return;
  _technique.addToDrawPlan(buildInfo);
}

void BackgroundDrawable::setLuminanceTexture(
                                  std::shared_ptr<CubeTexture> texture) noexcept
{
  if((_luminanceSampler.attachedTexture(0) != nullptr) != (texture != nullptr))
  {
    _technique.invalidate();
  }

  _luminanceSampler.setAttachedTexture(std::move(texture), 0);
}
