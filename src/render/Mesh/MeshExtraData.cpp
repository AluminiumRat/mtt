#include <algorithm>

#include <mtt/render/Mesh/AbstractMeshTechnique.h>
#include <mtt/render/Mesh/MeshExtraData.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

MeshExtraData::MeshExtraData(LogicalDevice& device) :
  _device(device)
{
}

MeshExtraData::~MeshExtraData() noexcept
{
  for(BufferRecord& record : _uniforms) delete record.buffer;
  for(SamplerRecord& record : _samplers) delete record.sampler;
  for(VariableRecord& record : _variables) delete record.variable;
}

void MeshExtraData::addUniformBuffer( std::unique_ptr<Buffer> buffer,
                                      const std::string& name)
{
  Uniforms::iterator iUniform = std::find_if( _uniforms.begin(),
                                              _uniforms.end(),
                                              [&](const BufferRecord& record)
                                                                        -> bool
                                              {
                                                return record.name == name;
                                              });
  if(iUniform != _uniforms.end()) Abort("MeshExtraData::addUniformBuffer: uniform with this name is already registered.");

  try
  {
    for(AbstractMeshTechnique* technique : _techniques)
    {
      technique->registerUniformBuffer(*buffer, name);
    }
  }
  catch(...)
  {
    Abort("MeshExtraData::addUniformBuffer: unable to register uniform buffer in technique.");
  }

  try
  {
    BufferRecord newRecord;
    newRecord.name = name;
    newRecord.buffer = buffer.get();
    _uniforms.push_back(newRecord);
    buffer.release();
  }
  catch (...)
  {
    Abort("MeshExtraData::addUniformBuffer: unable to register uniform buffer in mesh extra data.");
  }
}

std::unique_ptr<Buffer> MeshExtraData::removeUniformBuffer(size_t index) noexcept
{
  BufferRecord& record = _uniforms[index];

  for(AbstractMeshTechnique* technique : _techniques)
  {
    technique->unregisterUniformBuffer(*record.buffer, record.name);
  }
  std::unique_ptr<Buffer> buffer(_uniforms[index].buffer);
  _uniforms.erase(_uniforms.begin() + index);
  return buffer;
}

std::unique_ptr<Buffer> MeshExtraData::removeUniformBuffer(
                                              const std::string& name) noexcept
{
  for(size_t uniformIndex = 0; uniformIndex < _uniforms.size(); uniformIndex++)
  {
    if(_uniforms[uniformIndex].name == name)
    {
      return removeUniformBuffer(uniformIndex);
    }
  }
  return nullptr;
}

void MeshExtraData::setSurfaceMaterialData(const SurfaceMaterialData& newData)
{
  try
  {
    Buffer* buffer = uniformBuffer(surfaceMaterialUniformName);
    if(buffer != nullptr)
    {
      buffer->setData(&newData, sizeof(SurfaceMaterialData));
    }
    else
    {
      std::unique_ptr<Buffer> newBuffer(new Buffer( _device,
                                                    Buffer::UNIFORM_BUFFER));
      newBuffer->setData(&newData, sizeof(SurfaceMaterialData));
      addUniformBuffer(std::move(newBuffer), surfaceMaterialUniformName);
    }

    removeVariable(surfaceMaterialVariableName);
    addVariable(std::make_unique<SurfaceMaterialVariableType>(newData),
                surfaceMaterialVariableName);
  }
  catch(std::exception& error)
  {
    Log() << error.what();
    Abort("MeshExtraData::setSurfaceMaterialData: unable to update data.");
  }
  catch(...)
  {
    Abort("MeshExtraData::setSurfaceMaterialData: unknown error.");
  }
}

void MeshExtraData::removeSurfaceMaterialData() noexcept
{
  removeUniformBuffer(surfaceMaterialUniformName);
  removeVariable(surfaceMaterialVariableName);
}

void MeshExtraData::addSampler( std::unique_ptr<Sampler> sampler,
                                const std::string& name)
{
  Samplers::iterator iSampler = std::find_if( _samplers.begin(),
                                              _samplers.end(),
                                              [&](const SamplerRecord& record)
                                                                        -> bool
                                              {
                                                return record.name == name;
                                              });
  if(iSampler != _samplers.end()) Abort("MeshExtraData::addSampler: sampler with this name is already registered.");

  try
  {
    for(AbstractMeshTechnique* technique : _techniques)
    {
      technique->registerSampler(*sampler, name);
    }
  }
  catch(...)
  {
    Abort("MeshExtraData::addSampler: unable to register sampler in technique.");
  }

  try
  {
    SamplerRecord newRecord;
    newRecord.name = name;
    newRecord.sampler = sampler.get();
    _samplers.push_back(newRecord);
    sampler.release();
  }
  catch (...)
  {
    Abort("MeshExtraData::addSampler: unable to register sampler in mesh extra data.");
  }
}

std::unique_ptr<Sampler> MeshExtraData::removeSampler(size_t index) noexcept
{
  SamplerRecord& record = _samplers[index];

  for(AbstractMeshTechnique* technique : _techniques)
  {
    technique->unregisterSampler(*record.sampler, record.name);
  }

  std::unique_ptr<Sampler> sampler(_samplers[index].sampler);
  _samplers.erase(_samplers.begin() + index);
  return sampler;
}

std::unique_ptr<Sampler> MeshExtraData::removeSampler(
                                              const std::string& name) noexcept
{
  for (size_t samplerIndex = 0; samplerIndex < _samplers.size(); samplerIndex++)
  {
    if (_samplers[samplerIndex].name == name)
    {
      return removeSampler(samplerIndex);
    }
  }
  return nullptr;
}

void MeshExtraData::setAlbedoSampler(std::unique_ptr<Sampler> sampler)
{
  removeAlbedoSampler();
  addSampler(std::move(sampler), albedoSamplerName);
}

void MeshExtraData::removeAlbedoSampler() noexcept
{
  removeSampler(albedoSamplerName);
}

void MeshExtraData::setOpaqueSampler(std::unique_ptr<Sampler> sampler)
{
  removeOpaqueSampler();
  addSampler(std::move(sampler), opaqueSamplerName);
}

void MeshExtraData::removeOpaqueSampler() noexcept
{
  removeSampler(opaqueSamplerName);
}

void MeshExtraData::setSpecularSampler(std::unique_ptr<Sampler> sampler)
{
  removeSpecularSampler();
  addSampler(std::move(sampler), specularSamplerName);
}

void MeshExtraData::removeSpecularSampler() noexcept
{
  removeSampler(specularSamplerName);
}

void MeshExtraData::setNormalSampler(std::unique_ptr<Sampler> sampler)
{
  removeNormalSampler();
  addSampler(std::move(sampler), normalSamplerName);
}

void MeshExtraData::removeNormalSampler() noexcept
{
  removeSampler(normalSamplerName);
}

void MeshExtraData::setEmissionSampler(std::unique_ptr<Sampler> sampler)
{
  removeEmissionSampler();
  addSampler(std::move(sampler), emissionSamplerName);
}

void MeshExtraData::removeEmissionSampler() noexcept
{
  removeSampler(emissionSamplerName);
}

void MeshExtraData::setReflectionSampler(std::unique_ptr<Sampler> sampler)
{
  removeReflectionSampler();
  addSampler(std::move(sampler), reflectionSamplerName);
}

void MeshExtraData::removeReflectionSampler() noexcept
{
  removeSampler(reflectionSamplerName);
}

void MeshExtraData::addVariable(std::unique_ptr<AbstractMeshVariable> variable,
                                const std::string& name)
{
  Variables::iterator iVariable = std::find_if(
                                              _variables.begin(),
                                              _variables.end(),
                                              [&](const VariableRecord& record)
                                              -> bool
                                              {
                                                return record.name == name;
                                              });
  if (iVariable != _variables.end()) Abort("MeshExtraData::addVariable: variable with this name is already registered.");

  try
  {
    for (AbstractMeshTechnique* technique : _techniques)
    {
      technique->registerVariable(*variable, name);
    }
  }
  catch (...)
  {
    Abort("MeshExtraData::addVariable: unable to register variable in technique.");
  }

  try
  {
    VariableRecord newRecord;
    newRecord.name = name;
    newRecord.variable = variable.get();
    _variables.push_back(newRecord);
    variable.release();
  }
  catch (...)
  {
    Abort("MeshExtraData::addVariable: unable to register variable in mesh extra data.");
  }
}

std::unique_ptr<AbstractMeshVariable>
                          MeshExtraData::removeVariable(size_t index) noexcept
{
  VariableRecord& record = _variables[index];

  for (AbstractMeshTechnique* technique : _techniques)
  {
    technique->unregisterVariable(*record.variable, record.name);
  }

  std::unique_ptr<AbstractMeshVariable> variable(record.variable);
  _variables.erase(_variables.begin() + index);
  return variable;
}

std::unique_ptr<AbstractMeshVariable> MeshExtraData::removeVariable(
                                              const std::string& name) noexcept
{
  for ( size_t variableIndex = 0;
        variableIndex < _variables.size();
        variableIndex++)
  {
    if (_variables[variableIndex].name == name)
    {
      return removeVariable(variableIndex);
    }
  }
  return nullptr;
}

void MeshExtraData::setBoneInverseMatricesData(
                                        const std::vector<glm::mat4>& matrices)
{
  try
  {
    Buffer* buffer = uniformBuffer(boneInverseMatricesUniformName);
    if(buffer != nullptr)
    {
      buffer->setData(matrices.data(),
                      matrices.size() * sizeof(glm::mat4));
    }
    else
    {
      std::unique_ptr<Buffer> newBuffer(new Buffer( _device,
                                                    Buffer::UNIFORM_BUFFER));
      newBuffer->setData( matrices.data(),
                          matrices.size() * sizeof(glm::mat4));
      addUniformBuffer(std::move(newBuffer), boneInverseMatricesUniformName);
    }

    removeVariable(boneNumberVariableName);
    addVariable(std::make_unique<BoneNumberVariableType>(int(matrices.size())),
                boneNumberVariableName);
  }
  catch(std::exception& error)
  {
    Log() << error.what();
    Abort("MeshExtraData::setBoneInverseMatricesData: unable to set data.");
  }
  catch(...)
  {
    Abort("MeshExtraData::setBoneInverseMatricesData: unable to set data.");
  }
}

void MeshExtraData::removeBoneInverseMatricesData() noexcept
{
  removeUniformBuffer(boneInverseMatricesUniformName);
  removeVariable(boneNumberVariableName);
}

void MeshExtraData::setAlphaInAlbedoSamplerIsOpacity(bool value)
{
  removeAlphaInAlbedoSamplerIsOpacityVariable();
  addVariable(std::make_unique<MeshBoolVariable>(value),
              alphaInAlbedoSamplerIsOpacityVariableName);
}

void MeshExtraData::removeAlphaInAlbedoSamplerIsOpacityVariable() noexcept
{
  removeVariable(alphaInAlbedoSamplerIsOpacityVariableName);
}

void MeshExtraData::registerTechnique(AbstractMeshTechnique& technique)
{
  Techniques::iterator iTechnique = std::find(_techniques.begin(),
                                              _techniques.end(),
                                              &technique);
  if(iTechnique != _techniques.end()) Abort("MeshExtraData::registerTechnique: technique is already registered.");

  try
  {
    for(BufferRecord& record : _uniforms)
    {
      technique.registerUniformBuffer(*record.buffer, record.name);
    }

    for(SamplerRecord& record : _samplers)
    {
      technique.registerSampler(*record.sampler, record.name);
    }

    for(VariableRecord& record : _variables)
    {
      technique.registerVariable(*record.variable, record.name);
    }

    _techniques.push_back(&technique);
  }
  catch(std::exception& error)
  {
    Log() << error.what();
    Abort("MeshExtraData::registerTechnique: unable to register technique.");
  }
  catch(...)
  {
    Abort("MeshExtraData::registerTechnique: unable to register technique.");
  }
}

void MeshExtraData::unRegisterTechnique(
                                      AbstractMeshTechnique& technique) noexcept
{
  Techniques::iterator iTechnique = std::find(_techniques.begin(),
                                              _techniques.end(),
                                              &technique);
  if(iTechnique == _techniques.end()) Abort("MeshExtraData::unRegisterTechnique: technique is not registered.");

  for (BufferRecord& uniform : _uniforms)
  {
    technique.unregisterUniformBuffer(*uniform.buffer, uniform.name);
  }

  for (SamplerRecord& sampler : _samplers)
  {
    technique.unregisterSampler(*sampler.sampler, sampler.name);
  }

  for (VariableRecord& record : _variables)
  {
    technique.unregisterVariable(*record.variable, record.name);
  }

  _techniques.erase(iTechnique);
}
