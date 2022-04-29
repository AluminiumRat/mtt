#include <mtt/clPipeline/RenderPass/ForwardLightPass.h>

#include <Render/Particles/ColorDrawParticlesAction.h>

ColorDrawParticlesAction::ColorDrawParticlesAction(
                      mtt::GraphicsPipeline& pipeline,
                      VkViewport viewport,
                      VkRect2D scissor,
                      uint32_t pointsNumber,
                      mtt::PlainBuffer& indicesBuffer,
                      mtt::VolatileUniform<mtt::DrawMatrices>& matricesUniform,
                      const mtt::DrawMatrices& drawMatrices,
                      mtt::Texture2D& depthSamplerTexture) :
  _pipeline(pipeline),
  _viewport(viewport),
  _scissor(scissor),
  _pointsNumber(pointsNumber),
  _indicesBuffer(&indicesBuffer),
  _matricesUniform(matricesUniform),
  _drawMatrices(drawMatrices),
  _depthSamplerTexture(depthSamplerTexture)
{
}

void ColorDrawParticlesAction::execute(mtt::DrawContext& context)
{
  _matricesUniform.setValuePtr(&_drawMatrices);

  _pipeline.setScissor(_scissor);
  _pipeline.setViewport(_viewport);

  constexpr uint32_t depthSamplerIndex =
                        mtt::clPipeline::ForwardLightPass::depthSamplerIndex;

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
