#include <mtt/dlPipeline/MeshTechniques/TransparentColorTechnique.h>
#include <mtt/dlPipeline/RenderPass/PeelingGPass.h>
#include <mtt/dlPipeline/constants.h>
#include <mtt/render/Mesh/MeshExtraData.h>

using namespace mtt;
using namespace mtt::dlPipeline;

TransparentColorTechnique::TransparentColorTechnique(
                                                VkPrimitiveTopology topology) :
  BaseGeometryTechnique(NORMALS_FEATURE |
                          MATERIAL_FEATURE |
                          TEX_COORDS_FEATURE |
                          SKELETON_FEATURE |
                          ALBEDO_SAMPLER_FEATURE |
                          OPAQUE_SAMPLER_FEATURE |
                          SPECULAR_SAMPLER_FEATURE |
                          NORMAL_SAMPLER_FEATURE,
                        peelingGBufferStage,
                        topology,
                        NEAR_FIRST_ORDER)
{
}

void TransparentColorTechnique::registerVariable(
                                                AbstractMeshVariable& variable,
                                                const std::string& name)
{
  BaseGeometryTechnique::registerVariable(variable, name);
  if(name == MeshExtraData::alphaInAlbedoSamplerIsOpacityVariableName)
  {
    MeshBoolVariable& useAlphaVariable =
                                      static_cast<MeshBoolVariable&>(variable);
    invalidatePipeline();
  }
}

void TransparentColorTechnique::unregisterVariable(
                                              AbstractMeshVariable& variable,
                                              const std::string& name) noexcept
{
  BaseGeometryTechnique::unregisterVariable(variable, name);
  if (name == MeshExtraData::alphaInAlbedoSamplerIsOpacityVariableName)
  {
    invalidatePipeline();
  }
}

void TransparentColorTechnique::invalidatePipeline() noexcept
{
  BaseGeometryTechnique::invalidatePipeline();
  _frontBufferAttachment.reset();
  _backBufferAttachment.reset();
}

void TransparentColorTechnique::adjustPipeline( GraphicsPipeline& pipeline,
                                                AbstractRenderPass& renderPass)
{
  BaseGeometryTechnique::adjustPipeline(pipeline, renderPass);

  LogicalDevice& device = pipeline.device();

  _frontBufferAttachment.emplace(device);
  pipeline.addResource( "frontBufferBinding",
                        *_frontBufferAttachment,
                        VK_SHADER_STAGE_FRAGMENT_BIT);

  _backBufferAttachment.emplace(device);
  pipeline.addResource( "backBufferBinding",
                        *_backBufferAttachment,
                        VK_SHADER_STAGE_FRAGMENT_BIT);

  _backBufferAttachment.emplace(device);

  pipeline.setDepthTestEnable(true);
  pipeline.setDepthWriteEnable(true);
  pipeline.setDepthCompareOp(VK_COMPARE_OP_GREATER_OR_EQUAL);

  std::unique_ptr<ShaderModule> fragmentShader(
                                new ShaderModule( ShaderModule::FRAGMENT_SHADER,
                                                  renderPass.device()));
  fragmentShader->newFragment().loadFromFile("dlPipeline/meshGBuffer.frag");
  fragmentShader->setDefine("ENABLE_DEPTH_PEELING");
  fragmentShader->setDefine("ENABLE_ALPHA_TEST");
  pipeline.addShader(std::move(fragmentShader));
}

void TransparentColorTechnique::createRenderAction(
                                              DrawPlanBuildInfo& buildInfo,
                                              GraphicsPipeline& pipeline,
                                              MatricesUniform& matricesUniform)
{
  if(!normalsEnabled()) return;
  if(!materialDataEnabled()) return;

  createActionTemplate( buildInfo,
                        pipeline,
                        matricesUniform,
                        *_frontBufferAttachment,
                        PeelingGPass::frontAttachmentIndex,
                        *_backBufferAttachment,
                        PeelingGPass::backAttachmentIndex);
}
