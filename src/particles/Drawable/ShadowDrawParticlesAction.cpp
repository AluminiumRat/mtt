#include <mtt/clPipeline/RenderPass/ForwardLightPass.h>
#include <mtt/particles/Drawable/ShadowDrawParticlesAction.h>

using namespace mtt;

ShadowDrawParticlesAction::ShadowDrawParticlesAction(
                    GraphicsPipeline& pipeline,
                    VkViewport viewport,
                    VkRect2D scissor,
                    uint32_t pointsNumber,
                    PlainBuffer& indicesBuffer,
                    VolatileUniform<DrawMatrices>& matricesUniform,
                    const DrawMatrices& drawMatrices,
                    MaxDistanceUniform& maxDistanceUniform,
                    float maxDistance,
                    VolatileUniform<MppxDistanceFunction>& mppxFunctionUniform,
                    MppxDistanceFunction mppxFunctionValues,
                    VolatileUniform<glm::vec2>& falloffUniform,
                    glm::vec2 falloffValue) :
  _pipeline(pipeline),
  _viewport(viewport),
  _scissor(scissor),
  _pointsNumber(pointsNumber),
  _indicesBuffer(&indicesBuffer),
  _matricesUniform(matricesUniform),
  _drawMatrices(drawMatrices),
  _maxDistanceUniform(maxDistanceUniform),
  _maxDistance(maxDistance),
  _mppxFunctionUniform(mppxFunctionUniform),
  _mppxFunctionValues(mppxFunctionValues),
  _falloffUniform(falloffUniform),
  _falloffValue(falloffValue)
{
}

void ShadowDrawParticlesAction::execute(DrawContext& context)
{
  _matricesUniform.setValuePtr(&_drawMatrices);
  _maxDistanceUniform.setValuePtr(&_maxDistance);
  _mppxFunctionUniform.setValuePtr(&_mppxFunctionValues);
  _falloffUniform.setValuePtr(&_falloffValue);

  _pipeline.setScissor(_scissor);
  _pipeline.setViewport(_viewport);

  constexpr uint32_t depthSamplerIndex =
                                clPipeline::ForwardLightPass::depthSamplerIndex;

  _pipeline.scheduleBind(context.drawProducer);
  _pipeline.indices().scheduleBindData(*_indicesBuffer, context.drawProducer);

  vkCmdDrawIndexed( context.drawProducer.bufferHandle(),
                    _pointsNumber,
                    1,
                    0,
                    0,
                    0);
}
