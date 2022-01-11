#include <mtt/Render/Mesh/AbstractMeshTechnique.h>
#include <mtt/Render/Mesh/MeshExtraData.h>

using namespace mtt;

AbstractMeshTechnique::AbstractMeshTechnique() :
  _verticesNumber(0),
  _meshExtraData(nullptr)
{
}

AbstractMeshTechnique::~AbstractMeshTechnique() noexcept
{
}

void AbstractMeshTechnique::setVerticesNumber(uint32_t newValue)
{
  _verticesNumber = newValue;
}

void AbstractMeshTechnique::setMeshExtraData(MeshExtraData* newData)
{
  if(_meshExtraData != nullptr) _meshExtraData->unRegisterTechnique(*this);
  _meshExtraData = nullptr;
  if (newData != nullptr) newData->registerTechnique(*this);
  _meshExtraData = newData;
}

void AbstractMeshTechnique::registerVertexBuffer( Buffer& buffer,
                                                  const std::string& name)
{
}

void AbstractMeshTechnique::unregisterVertexBuffer(
                                              Buffer& buffer,
                                              const std::string& name)  noexcept
{
}

void AbstractMeshTechnique::registerIndicesBuffer(VkPrimitiveTopology topology,
                                                  Buffer& buffer,
                                                  VkIndexType indexType,
                                                  size_t indicesNumber)
{
}

void AbstractMeshTechnique::unregisterIndicesBuffer(
                                                  VkPrimitiveTopology topology)
{
}

void AbstractMeshTechnique::registerUniformBuffer(Buffer& buffer,
                                                  const std::string& name)
{
}

void AbstractMeshTechnique::unregisterUniformBuffer(
                                              Buffer& buffer,
                                              const std::string& name)  noexcept
{
}

void AbstractMeshTechnique::registerSampler(Sampler& sampler,
                                            const std::string& name)
{
}

void AbstractMeshTechnique::unregisterSampler(Sampler& sampler,
                                              const std::string& name)  noexcept
{
}

void AbstractMeshTechnique::registerVariable( AbstractMeshVariable& variable,
                                              const std::string& name)
{
}

void AbstractMeshTechnique::unregisterVariable(
                                              AbstractMeshVariable& variable,
                                              const std::string& name) noexcept
{
}