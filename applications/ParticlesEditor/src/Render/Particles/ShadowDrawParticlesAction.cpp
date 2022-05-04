#include <mtt/clPipeline/RenderPass/ForwardLightPass.h>

#include <Render/Particles/ShadowDrawParticlesAction.h>

ShadowDrawParticlesAction::ShadowDrawParticlesAction(
                      mtt::GraphicsPipeline& pipeline,
                      VkViewport viewport,
                      VkRect2D scissor,
                      uint32_t pointsNumber,
                      mtt::PlainBuffer& indicesBuffer,
                      mtt::VolatileUniform<mtt::DrawMatrices>& matricesUniform,
                      const mtt::DrawMatrices& drawMatrices,
                      NearFarUniform& nearFarUniform,
                      NearFarInfo nearfarData,
                      mtt::VolatileUniform<glm::vec2>& falloffUniform,
                      glm::vec2 falloffValue) :
  _pipeline(pipeline),
  _viewport(viewport),
  _scissor(scissor),
  _pointsNumber(pointsNumber),
  _indicesBuffer(&indicesBuffer),
  _matricesUniform(matricesUniform),
  _drawMatrices(drawMatrices),
  _nearFarUniform(nearFarUniform),
  _nearfarData(nearfarData),
  _falloffUniform(falloffUniform),
  _falloffValue(falloffValue)
{
}

void ShadowDrawParticlesAction::execute(mtt::DrawContext& context)
{
  _matricesUniform.setValuePtr(&_drawMatrices);
  _nearFarUniform.setValuePtr(&_nearfarData);
  _falloffUniform.setValuePtr(&_falloffValue);

  _pipeline.setScissor(_scissor);
  _pipeline.setViewport(_viewport);

  constexpr uint32_t depthSamplerIndex =
                        mtt::clPipeline::ForwardLightPass::depthSamplerIndex;

  _pipeline.scheduleBind(context.drawProducer);
  _pipeline.indices().scheduleBindData(*_indicesBuffer, context.drawProducer);

  vkCmdDrawIndexed( context.drawProducer.bufferHandle(),
                    _pointsNumber,
                    1,
                    0,
                    0,
                    0);
}
