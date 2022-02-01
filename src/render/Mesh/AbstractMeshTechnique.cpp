#include <mtt/render/Mesh/AbstractMeshTechnique.h>
#include <mtt/render/Mesh/Mesh.h>

using namespace mtt;

AbstractMeshTechnique::AbstractMeshTechnique() :
  _mesh(nullptr),
  _verticesNumber(0)
{
}

AbstractMeshTechnique::~AbstractMeshTechnique() noexcept
{
}

void AbstractMeshTechnique::setMesh(Mesh* newMesh)
{
  if(_mesh != nullptr) _mesh->releaseTechnique(*this);
  if (newMesh != nullptr) newMesh->bindTechnique(*this);
  _mesh = newMesh;
}

void AbstractMeshTechnique::setVerticesNumber(uint32_t newValue)
{
  _verticesNumber = newValue;
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
                                        VkPrimitiveTopology topology) noexcept
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

void AbstractMeshTechnique::registerAreaModificators(AreaModificatorSet& set)
{
}

void AbstractMeshTechnique::unregisterAreaModificators(
                                              AreaModificatorSet& set) noexcept
{
}
