#include <mtt/Render/DrawPlan/AbstractAction.h>
#include <mtt/Render/DrawPlan/DrawMeshAction.h>
#include <mtt/Render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/Render/Mesh/BaseGeometryTechnique.h>
#include <mtt/Render/Mesh/Mesh.h>
#include <mtt/Render/RenderPass/GeneralRenderPass.h>
#include <mtt/Utilities/Abort.h>

using namespace mtt;

BaseGeometryTechnique::BaseGeometryTechnique( int availableFeatures,
                                              StageIndex stage,
                                              VkPrimitiveTopology topology,
                                              PriorityOrder priorityOrder) :
  MonotopologyMeshTechnique(topology),
  _availableFeatures(availableFeatures),
  _stage(stage),
  _priorityOrder(priorityOrder),
  _positionsBuffer(nullptr),
  _normalBuffer(nullptr),
  _tangentBuffer(nullptr),
  _binormalBuffer(nullptr),
  _texCoordBuffer(nullptr),
  _boneIndicesBuffer(nullptr),
  _boneWeightsBuffer(nullptr),
  _materialUniformBuffer(nullptr),
  _albedoSampler(nullptr),
  _opaqueSampler(nullptr),
  _specularSampler(nullptr),
  _normalSampler(nullptr),
  _emissiveSampler(nullptr),
  _reflectionSampler(nullptr),
  _boneNumber(0),
  _boneMatricesPtr(nullptr),
  _boneInverseMatricesUniformBuffer(nullptr)
{
}

void BaseGeometryTechnique::registerVertexBuffer( Buffer& buffer,
                                                  const std::string& name)
{
  if(name == Mesh::positionBufferName)
  {
    _positionsBuffer = &buffer;
    invalidatePipeline();
  }
  else if(name == Mesh::normalBufferName)
  {
    if( _availableFeatures & NORMALS_FEATURE ||
        _availableFeatures & REFLECTION_SAMPLER_FEATURE)
    {
      _normalBuffer = &buffer;
      invalidatePipeline();
    }
  }
  else if(name == Mesh::tangentBufferName)
  {
    if(_availableFeatures & NORMAL_SAMPLER_FEATURE)
    {
      _tangentBuffer = &buffer;
      invalidatePipeline();
    }
  }
  else if(name == Mesh::binormalBufferName)
  {
    if(_availableFeatures & NORMAL_SAMPLER_FEATURE)
    {
      _binormalBuffer = &buffer;
      invalidatePipeline();
    }
  }
  else if(name == Mesh::texCoordBufferName)
  {
    if(_availableFeatures & TEX_COORDS_FEATURE)
    {
      _texCoordBuffer = &buffer;
      invalidatePipeline();
    }
  }
  else if(name == Mesh::boneIndicesBufferName)
  {
    if(_availableFeatures & SKELETON_FEATURE)
    {
      _boneIndicesBuffer = &buffer;
      invalidatePipeline();
    }
  }
  else if(name == Mesh::boneWeightsBufferName)
  {
    if(_availableFeatures & SKELETON_FEATURE)
    {
      _boneWeightsBuffer = &buffer;
      invalidatePipeline();
    }
  }
}

void BaseGeometryTechnique::unregisterVertexBuffer(
                                              Buffer& buffer,
                                              const std::string& name) noexcept
{
  if(name == Mesh::positionBufferName)
  {
    _positionsBuffer = nullptr;
    invalidatePipeline();
  }
  else if(name == Mesh::normalBufferName)
  {
    if(_availableFeatures & NORMALS_FEATURE)
    {
      _normalBuffer = nullptr;
      invalidatePipeline();
    }
  }
  else if(name == Mesh::tangentBufferName)
  {
    if(_availableFeatures & NORMAL_SAMPLER_FEATURE)
    {
      _tangentBuffer = nullptr;
      invalidatePipeline();
    }
  }
  else if(name == Mesh::binormalBufferName)
  {
    if(_availableFeatures & NORMAL_SAMPLER_FEATURE)
    {
      _binormalBuffer = nullptr;
      invalidatePipeline();
    }
  }
  else if(name == Mesh::texCoordBufferName)
  {
    if(_availableFeatures & TEX_COORDS_FEATURE)
    {
      _texCoordBuffer = nullptr;
      invalidatePipeline();
    }
  }
  else if(name == Mesh::boneIndicesBufferName)
  {
    if(_availableFeatures & SKELETON_FEATURE)
    {
      _boneIndicesBuffer = nullptr;
      invalidatePipeline();
    }
  }
  else if(name == Mesh::boneWeightsBufferName)
  {
    if(_availableFeatures & SKELETON_FEATURE)
    {
      _boneWeightsBuffer = nullptr;
      invalidatePipeline();
    }
  }
}

void BaseGeometryTechnique::registerUniformBuffer(Buffer& buffer,
                                                  const std::string& name)
{
  if( name == MeshExtraData::surfaceMaterialUniformName)
  {
    if(_availableFeatures& MATERIAL_FEATURE)
    {
      _materialUniformBuffer = &buffer;
      invalidatePipeline();
    }
  }
  else if (name == MeshExtraData::boneInverseMatricesUniformName)
  {
    if (_availableFeatures & SKELETON_FEATURE)
    {
      _boneInverseMatricesUniformBuffer = &buffer;
      invalidatePipeline();
    }
  }
}

void BaseGeometryTechnique::unregisterUniformBuffer(
                                              Buffer& buffer,
                                              const std::string& name) noexcept
{
  if(name == MeshExtraData::surfaceMaterialUniformName)
  {
    if(_availableFeatures & MATERIAL_FEATURE)
    {
      _materialUniformBuffer = nullptr;
      invalidatePipeline();
    }
  }
  else if(name == MeshExtraData::boneInverseMatricesUniformName)
  {
    if(_availableFeatures & SKELETON_FEATURE)
    {
      _boneInverseMatricesUniformBuffer = nullptr;
      invalidatePipeline();
    }
  }
}

void BaseGeometryTechnique::registerSampler(Sampler& sampler,
                                            const std::string& name)
{
  if(name == MeshExtraData::albedoSamplerName)
  {
    if(_availableFeatures & ALBEDO_SAMPLER_FEATURE)
    {
      _albedoSampler = &sampler;
      invalidatePipeline();
    }
  }
  else if(name == MeshExtraData::opaqueSamplerName)
  {
    if(_availableFeatures & OPAQUE_SAMPLER_FEATURE)
    {
      _opaqueSampler = &sampler;
      invalidatePipeline();
    }
  }
  else if (name == MeshExtraData::specularSamplerName)
  {
    if(_availableFeatures & SPECULAR_SAMPLER_FEATURE)
    {
      _specularSampler = &sampler;
      invalidatePipeline();
    }
  }
  else if (name == MeshExtraData::normalSamplerName)
  {
    if(_availableFeatures & NORMAL_SAMPLER_FEATURE)
    {
      _normalSampler = &sampler;
      invalidatePipeline();
    }
  }
  else if (name == MeshExtraData::emissionSamplerName)
  {
    if(_availableFeatures & EMISSION_SAMPLER_FEATURE)
    {
      _emissiveSampler = &sampler;
      invalidatePipeline();
    }
  }
  else if (name == MeshExtraData::reflectionSamplerName)
  {
    if(_availableFeatures & REFLECTION_SAMPLER_FEATURE)
    {
      _reflectionSampler = &sampler;
      invalidatePipeline();
    }
  }
}

void BaseGeometryTechnique::unregisterSampler(Sampler& sampler,
                                              const std::string& name) noexcept
{
  if (name == MeshExtraData::albedoSamplerName)
  {
    if (_availableFeatures & ALBEDO_SAMPLER_FEATURE)
    {
      _albedoSampler = nullptr;
      invalidatePipeline();
    }
  }
  if (name == MeshExtraData::opaqueSamplerName)
  {
    if (_availableFeatures & OPAQUE_SAMPLER_FEATURE)
    {
      _opaqueSampler = nullptr;
      invalidatePipeline();
    }
  }
  else if(name == MeshExtraData::specularSamplerName)
  {
    if (_availableFeatures & SPECULAR_SAMPLER_FEATURE)
    {
      _specularSampler = nullptr;
      invalidatePipeline();
    }
  }
  else if(name == MeshExtraData::normalSamplerName)
  {
    if (_availableFeatures & NORMAL_SAMPLER_FEATURE)
    {
      _normalSampler = nullptr;
      invalidatePipeline();
    }
  }
  else if(name == MeshExtraData::emissionSamplerName)
  {
    if (_availableFeatures & EMISSION_SAMPLER_FEATURE)
    {
      _emissiveSampler = nullptr;
      invalidatePipeline();
    }
  }
  else if(name == MeshExtraData::reflectionSamplerName)
  {
    if (_availableFeatures & REFLECTION_SAMPLER_FEATURE)
    {
      _reflectionSampler = nullptr;
      invalidatePipeline();
    }
  }
}

void BaseGeometryTechnique::registerVariable( AbstractMeshVariable& variable,
                                              const std::string& name)
{
  if (_availableFeatures & SKELETON_FEATURE)
  {
    if(name == MeshExtraData::boneNumberVariableName)
    {
      MeshExtraData::BoneNumberVariableType& numberVariable =
                static_cast<MeshExtraData::BoneNumberVariableType&>(variable);
      _boneNumber = numberVariable.value();
      invalidatePipeline();
    }
    if(name == MeshExtraData::boneMatricesVariableName)
    {
      MeshExtraData::BoneMatricesVariableType& matricesVariable =
                static_cast<MeshExtraData::BoneMatricesVariableType&>(variable);
      _boneMatricesPtr = matricesVariable.value();
      invalidatePipeline();
    }
  }
}

void BaseGeometryTechnique::unregisterVariable(
                                              AbstractMeshVariable& variable,
                                              const std::string& name) noexcept
{
  if (_availableFeatures & SKELETON_FEATURE)
  {
    if(name == MeshExtraData::boneNumberVariableName)
    {
      _boneNumber = 0;
      invalidatePipeline();
    }
    if(name == MeshExtraData::boneMatricesVariableName)
    {
      _boneMatricesPtr = nullptr;
      invalidatePipeline();
    }
  }
}

void BaseGeometryTechnique::setIndicesBuffer( Buffer* buffer,
                                              VkIndexType indexType) noexcept
{
  invalidatePipeline();
}

void BaseGeometryTechnique::invalidatePipeline() noexcept
{
  _pipeline.reset();
  _matricesUniform.reset();
  _boneMatricesUniform.reset();
}

void BaseGeometryTechnique::_rebuildPipeline(AbstractRenderPass& renderPass)
{
  invalidatePipeline();
  _pipeline.emplace(renderPass, _stage);

  try
  {
    adjustPipeline(_pipeline.value(), renderPass);
  }
  catch(...)
  {
    invalidatePipeline();
    throw;
  }
}

void BaseGeometryTechnique::adjustPipeline( GraphicsPipeline& pipeline,
                                            AbstractRenderPass& renderPass)
{
  std::unique_ptr<ShaderModule> vertexShader(
                                  new ShaderModule( ShaderModule::VERTEX_SHADER,
                                                    renderPass.device()));
  vertexShader->newFragment().loadFromFile("meshCommon.vert");
  _pipeline->addShader(std::move(vertexShader));

  if(_positionsBuffer == nullptr) Abort("BaseGeometryTechnique::adjustPipeline: position buffer is null");
  VertexAttribute* positionAttribute =
                        &_pipeline->getOrCreateAttribute(
                                              Mesh::positionAttributeLocation);
  positionAttribute->adjustDataType(Mesh::positionAttributeType);
  positionAttribute->attachBuffer(_positionsBuffer);

  if(normalsEnabled() || reflectionMappingEnabled())
  {
    VertexAttribute* normalAttribute =
                              &_pipeline->getOrCreateAttribute(
                                                Mesh::normalAttributeLocation);
    normalAttribute->adjustDataType(Mesh::normalAttributeType);
    normalAttribute->attachBuffer(_normalBuffer);
  }
  if(normalsEnabled()) _pipeline->setDefine("ENABLE_NORMAL");

  if(texCoordEnabled())
  {
    _pipeline->setDefine("ENABLE_TEX_COORDS");
    VertexAttribute* texCoordAttribute =
                                &_pipeline->getOrCreateAttribute(
                                              Mesh::texCoordAttributeLocation);
    texCoordAttribute->adjustDataType(Mesh::texCoordAttributeType);
    texCoordAttribute->attachBuffer(_texCoordBuffer);
  }

  if(albedoMappingEnabled())
  {
    _pipeline->setDefine("ENABLE_DIFFUSE_TEXTURE");
    _pipeline->addResource( MeshExtraData::albedoSamplerBinding,
                            *_albedoSampler,
                            VK_SHADER_STAGE_FRAGMENT_BIT);
  }

  if(opaqueMappingEnabled())
  {
    _pipeline->setDefine("ENABLE_OPAQUE_TEXTURE");
    _pipeline->addResource( MeshExtraData::opaqueSamplerBinding,
                            *_opaqueSampler,
                            VK_SHADER_STAGE_FRAGMENT_BIT);
  }

  if(specularMappingEnabled())
  {
    _pipeline->setDefine("ENABLE_SPECULAR_TEXTURE");
    _pipeline->addResource( MeshExtraData::specularSamplerBinding,
                            *_specularSampler,
                            VK_SHADER_STAGE_FRAGMENT_BIT);
  }

  if(normalMappingEnabled())
  {
    _pipeline->setDefine("ENABLE_NORMAL_MAPPING");
    VertexAttribute* tangentAttribute =
                                  &_pipeline->getOrCreateAttribute(
                                                Mesh::tangentAttributeLocation);
    tangentAttribute->adjustDataType(Mesh::tangentAttributeType);
    tangentAttribute->attachBuffer(_tangentBuffer);

    VertexAttribute* binormalAttribute =
                                &_pipeline->getOrCreateAttribute(
                                              Mesh::binormalAttributeLocation);
    binormalAttribute->adjustDataType(Mesh::binormalAttributeType);
    binormalAttribute->attachBuffer(_binormalBuffer);

    _pipeline->addResource( MeshExtraData::normalSamplerBinding,
                            *_normalSampler,
                            VK_SHADER_STAGE_FRAGMENT_BIT);
  }

  if(emissiveMappingEnabled())
  {
    _pipeline->setDefine("ENABLE_EMISSION_TEXTURE");
    _pipeline->addResource( MeshExtraData::emissionSamplerBinding,
                            *_emissiveSampler,
                            VK_SHADER_STAGE_FRAGMENT_BIT);
  }

  if(reflectionMappingEnabled())
  {
    _pipeline->setDefine("ENABLE_REFLECTION");
    _pipeline->addResource( MeshExtraData::reflectionSamplerBinding,
                            *_reflectionSampler,
                            VK_SHADER_STAGE_FRAGMENT_BIT);
  }

  if(skeletonEnabled())
  {
    _pipeline->setDefine("SKELETON");
    _pipeline->setDefine("BONE_NUMBER", std::to_string(_boneNumber));

    VertexAttribute* indicesAttribute =
                                &_pipeline->getOrCreateAttribute(
                                            Mesh::boneIndicesAttributeLocation);
    indicesAttribute->adjustDataType(Mesh::boneIndicesAttributeType);
    indicesAttribute->attachBuffer(_boneIndicesBuffer);

    VertexAttribute* weightsAttribute =
          &_pipeline->getOrCreateAttribute(Mesh::boneWeightsAttributeLocation);
    weightsAttribute->adjustDataType(Mesh::boneWeightsAttributeType);
    weightsAttribute->attachBuffer(_boneWeightsBuffer);

    _pipeline->addResource( MeshExtraData::boneInverseMatricesUniformBinding,
                            *_boneInverseMatricesUniformBuffer,
                            VK_SHADER_STAGE_VERTEX_BIT);

    _boneMatricesUniform.emplace();
    _pipeline->addResource( MeshExtraData::boneMatricesUniformBinding,
                            _boneMatricesUniform.value(),
                            VK_SHADER_STAGE_VERTEX_BIT);
  }

  _matricesUniform.emplace();
  _pipeline->addResource( DrawMatrices::bindingName,
                          _matricesUniform.value(),
                          VK_SHADER_STAGE_VERTEX_BIT);

  if(materialDataEnabled())
  {
    _pipeline->setDefine("MATERIAL_ENABLED");
    _pipeline->addResource( MeshExtraData::surfaceMaterialUniformBinding,
                            *_materialUniformBuffer,
                            VK_SHADER_STAGE_FRAGMENT_BIT);
  }

  if(_availableFeatures & VIEW_COORDS_IN_FRAGMENT_SHADER_FEATURE)
  {
    _pipeline->setDefine("ENABLE_VIEW_COORDS_IN_FRAGMENT_SHADER");
  }

  if (indicesBuffer() != nullptr)
  {
    _pipeline->indices().attachBuffer(indicesBuffer());
    _pipeline->indices().setType(indexType());
  }
  _pipeline->setTopology(topology());
}

void BaseGeometryTechnique::addToDrawPlan(DrawPlanBuildInfo& buildInfo)
{
  if (_positionsBuffer == nullptr) return;
  AbstractRenderPass* renderPass = buildInfo.builder->stagePass(_stage);
  if(renderPass == nullptr) return;

  if (!_pipeline.has_value() || !_pipeline->isCompatible(*renderPass))
  {
    _rebuildPipeline(*renderPass);
  }

  if (skeletonEnabled() && _boneMatricesPtr == nullptr)
  {
    Log() << "BaseGeometryTechnique::addToDrawPlan: boneMatrices is null.";
    return;
  }

  createRenderAction( buildInfo,
                      _pipeline.value(),
                      _matricesUniform.value(),
                      _boneMatricesPtr);
}

void BaseGeometryTechnique::createRenderAction(
                                            DrawPlanBuildInfo& buildInfo,
                                            GraphicsPipeline& pipeline,
                                            MatricesUniform& matricesUniform,
                                            const BoneMatrices* boneMatricesPtr)
{
  createActionTemplate( buildInfo,
                        pipeline,
                        matricesUniform,
                        boneMatricesPtr);
}

float BaseGeometryTechnique::getCustomPriority(
                              const DrawPlanBuildInfo& buildInfo) const noexcept
{
  return 0;
}
