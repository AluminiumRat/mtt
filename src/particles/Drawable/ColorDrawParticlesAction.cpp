#include <mtt/clPipeline/RenderPass/ForwardLightPass.h>
#include <mtt/particles/Drawable/ColorDrawParticlesAction.h>

using namespace mtt;

ColorDrawParticlesAction::ColorDrawParticlesAction(
          GraphicsPipeline& pipeline,
          VkViewport viewport,
          VkRect2D scissor,
          uint32_t pointsNumber,
          PlainBuffer& indicesBuffer,
          VolatileUniform<DrawMatrices>& matricesUniform,
          const DrawMatrices& drawMatrices,
          VolatileUniform<MppxDistanceFunction>& mppxFunctionUniform,
          MppxDistanceFunction mppxFunctionValues,
          VolatileUniform<glm::vec2>& falloffUniform,
          glm::vec2 falloffValue,
          Texture2D& depthSamplerTexture) :
  _pipeline(pipeline),
  _viewport(viewport),
  _scissor(scissor),
  _pointsNumber(pointsNumber),
  _indicesBuffer(&indicesBuffer),
  _matricesUniform(matricesUniform),
  _drawMatrices(drawMatrices),
  _mppxFunctionUniform(mppxFunctionUniform),
  _mppxFunctionValues(mppxFunctionValues),
  _falloffUniform(falloffUniform),
  _falloffValue(falloffValue),
  _depthSamplerTexture(depthSamplerTexture)
{
}

void ColorDrawParticlesAction::execute(DrawContext& context)
{
  _matricesUniform.setValuePtr(&_drawMatrices);
  _mppxFunctionUniform.setValuePtr(&_mppxFunctionValues);
  _falloffUniform.setValuePtr(&_falloffValue);

  _pipeline.setScissor(_scissor);
  _pipeline.setViewport(_viewport);

  constexpr uint32_t depthSamplerIndex =
                                clPipeline::ForwardLightPass::depthSamplerIndex;

  _depthSamplerTexture.setImageView(
                            context.frameBuffer->samplerMap(depthSamplerIndex));

  _pipeline.scheduleBind(context.drawProducer);
  _pipeline.indices().scheduleBindData(*_indicesBuffer, context.drawProducer);

  vkCmdDrawIndexed( context.drawProducer.bufferHandle(),
                    _pointsNumber,
                    1,
                    0,
                    0,
                    0);
}
