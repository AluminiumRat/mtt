#include <mtt/clPipeline/Background/BackgroundDrawable.h>
#include <mtt/clPipeline/RenderPass/BackgroundPass.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/render/DrawPlan/DrawMeshAction.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;
using namespace mtt::clPipeline;

BackgroundDrawable::DrawTechnique::DrawTechnique(BackgroundDrawable& parent) :
  PipelineDrawable(colorFrameType, backgroundStage),
  _parent(parent)
{
}

void BackgroundDrawable::DrawTechnique::adjustPipeline(
                                                    GraphicsPipeline& pipeline)
{
  pipeline.addResource( "drawDataBinding",
                        _parent._drawDataUniform,
                        VK_SHADER_STAGE_FRAGMENT_BIT);

  pipeline.addResource( "drawMatricesBinding",
                        _parent._matricesUniform,
                        VK_SHADER_STAGE_FRAGMENT_BIT);

  pipeline.addResource( "depthMapBinding",
                        _parent._depthAttachment,
                        VK_SHADER_STAGE_FRAGMENT_BIT);

  if(_parent._luminanceSampler.attachedTexture(0) != nullptr)
  {
    pipeline.addResource( "luminanceTextureBinding",
                          _parent._luminanceSampler,
                          VK_SHADER_STAGE_FRAGMENT_BIT);
    pipeline.setDefine("LUMINANCE_SAMPLER_ENABLED");
  }

  std::unique_ptr<ShaderModule> vertexShader(
                                  new ShaderModule( ShaderModule::VERTEX_SHADER,
                                                    pipeline.device()));
  vertexShader->newFragment().loadFromFile(
                                          "clPipeline/backgroundDrawable.vert");
  pipeline.addShader(std::move(vertexShader));

  std::unique_ptr<ShaderModule> fragmentShader(
                                new ShaderModule( ShaderModule::FRAGMENT_SHADER,
                                                  pipeline.device()));
  fragmentShader->newFragment().loadFromFile(
                                          "clPipeline/backgroundDrawable.frag");
  pipeline.addShader(std::move(fragmentShader));

  pipeline.setTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP);

  pipeline.forceBuild();
}

void BackgroundDrawable::DrawTechnique::buildDrawActions(
                                                  DrawPlanBuildInfo& buildInfo)
{
  DrawBin* renderBin = buildInfo.currentFramePlan->getBin(stageIndex());
  if(renderBin == nullptr) Abort("BackgroundDrawable::DrawTechnique: background render bin is not supported.");

  uint32_t pointsNumber = 4;

  BackgroundDrawData drawData;
  drawData.properties = _parent._properties;
  drawData.viewToLocal = glm::inverse(buildInfo.drawMatrices.localToViewMatrix);

  using DrawAction = DrawMeshAction<VolatileUniform<BackgroundDrawData>,
                                    BackgroundDrawData,
                                    VolatileUniform<DrawMatrices>,
                                    DrawMatrices,
                                    InputAttachment,
                                    uint32_t>;
  renderBin->createAction<DrawAction>(0,
                                      *pipeline(),
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

BackgroundDrawable::BackgroundDrawable( const BackgroundProperties& properties,
                                        Sampler& luminanceSampler,
                                        LogicalDevice& device) :
  _device(device),
  _properties(properties),
  _depthAttachment(device),
  _luminanceSampler(luminanceSampler),
  _technique(*this)
{
}

void BackgroundDrawable::buildDrawActions(DrawPlanBuildInfo& buildInfo)
{
  _technique.addToDrawPlan(buildInfo);
}

void BackgroundDrawable::reset() noexcept
{
  _technique.resetPipeline();
}
