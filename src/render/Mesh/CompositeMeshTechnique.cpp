#include <mtt/render/Mesh/AbstractMeshTechnique.h>
#include <mtt/render/Mesh/CompositeMeshTechnique.h>

using namespace mtt;

void CompositeMeshTechnique::registerSubtechnique(
                                              AbstractMeshTechnique& technique)
{
  _subtechniques.push_back(&technique);
}

void CompositeMeshTechnique::setVerticesNumber(uint32_t newValue)
{
  for(AbstractMeshTechnique* subtechnique : _subtechniques)
  {
    subtechnique->setVerticesNumber(newValue);
  }
  AbstractMeshTechnique::setVerticesNumber(newValue);
}

void CompositeMeshTechnique::setMeshExtraData(MeshExtraData* newData)
{
  for (AbstractMeshTechnique* subtechnique : _subtechniques)
  {
    subtechnique->setMeshExtraData(newData);
  }
  AbstractMeshTechnique::setMeshExtraData(newData);
}

void CompositeMeshTechnique::registerVertexBuffer(Buffer& buffer,
                                                  const std::string& name)
{
  for (AbstractMeshTechnique* subtechnique : _subtechniques)
  {
    subtechnique->registerVertexBuffer(buffer, name);
  }
}

void CompositeMeshTechnique::unregisterVertexBuffer(
                                              Buffer& buffer,
                                              const std::string& name) noexcept
{
  for (AbstractMeshTechnique* subtechnique : _subtechniques)
  {
    subtechnique->unregisterVertexBuffer(buffer, name);
  }
}

void CompositeMeshTechnique::registerIndicesBuffer(
                                                  VkPrimitiveTopology topology,
                                                  Buffer& buffer,
                                                  VkIndexType indexType,
                                                  size_t indicesNumber)
{
  for (AbstractMeshTechnique* subtechnique : _subtechniques)
  {
    subtechnique->registerIndicesBuffer(topology,
                                        buffer,
                                        indexType,
                                        indicesNumber);
  }
}

void CompositeMeshTechnique::unregisterIndicesBuffer(
                                                  VkPrimitiveTopology topology)
{
  for (AbstractMeshTechnique* subtechnique : _subtechniques)
  {
    subtechnique->unregisterIndicesBuffer(topology);
  }
}

void CompositeMeshTechnique::registerUniformBuffer( Buffer& buffer,
                                                    const std::string& name)
{
}

void CompositeMeshTechnique::unregisterUniformBuffer(
                                              Buffer& buffer,
                                              const std::string& name) noexcept
{
}

void CompositeMeshTechnique::registerSampler( Sampler& sampler,
                                              const std::string& name)
{
}

void CompositeMeshTechnique::unregisterSampler(
                                              Sampler& sampler,
                                              const std::string& name) noexcept
{
}

void CompositeMeshTechnique::registerVariable(AbstractMeshVariable& variable,
                                              const std::string& name)
{
}

void CompositeMeshTechnique::unregisterVariable
                                              (AbstractMeshVariable& variable,
                                              const std::string& name) noexcept
{
}
