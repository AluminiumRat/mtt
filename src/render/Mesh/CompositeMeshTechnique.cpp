#include <algorithm>
#include <stdexcept>

#include <mtt/render/Mesh/AbstractMeshTechnique.h>
#include <mtt/render/Mesh/CompositeMeshTechnique.h>
#include <mtt/render/Mesh/Mesh.h>

using namespace mtt;

void CompositeMeshTechnique::registerSubtechnique(
                                              AbstractMeshTechnique& technique)
{
  _subtechniques.push_back(&technique);
  try
  {
    if(mesh() != nullptr) technique.setMesh(mesh());
  }
  catch (...)
  {
    _subtechniques.pop_back();
    throw;
  }
}

void CompositeMeshTechnique::unregisterSubtechnique(
                                              AbstractMeshTechnique& technique)
{
  Subtechniques::iterator iTechnique = std::find( _subtechniques.begin(),
                                                  _subtechniques.end(),
                                                  &technique);
  if(iTechnique == _subtechniques.end()) return;
  _subtechniques.erase(iTechnique);
  technique.setMesh(nullptr);
}

void CompositeMeshTechnique::setVerticesNumber(uint32_t newValue)
{
  try
  {
    AbstractMeshTechnique::setVerticesNumber(newValue);

    for(AbstractMeshTechnique* subtechnique : _subtechniques)
    {
      subtechnique->setVerticesNumber(newValue);
    }
  }
  catch (std::exception& error)
  {
    Log() << error.what();
    Abort("CompositeMeshTechnique::setVerticesNumber: unable to set vertices number.");
  }
  catch(...)
  {
    Abort("CompositeMeshTechnique::setVerticesNumber: unable to set vertices number.");
  }
}

void CompositeMeshTechnique::setMesh(Mesh* newMesh)
{
  try
  {
    AbstractMeshTechnique::setMesh(newMesh);

    for (AbstractMeshTechnique* subtechnique : _subtechniques)
    {
      subtechnique->setMesh(newMesh);
    }
  }
  catch (std::exception& error)
  {
    Log() << error.what();
    Abort("CompositeMeshTechnique::setMesh: unable to attach to mesh.");
  }
  catch(...)
  {
    Abort("CompositeMeshTechnique::setMesh: unable to attach to mesh.");
  }
}

void CompositeMeshTechnique::registerVertexBuffer(Buffer& buffer,
                                                  const std::string& name)
{
  try
  {
    for (AbstractMeshTechnique* subtechnique : _subtechniques)
    {
      subtechnique->registerVertexBuffer(buffer, name);
    }
  }
  catch (std::exception& error)
  {
    Log() << error.what();
    Abort("CompositeMeshTechnique::registerVertexBuffer: unable to register vertex buffer.");
  }
  catch(...)
  {
    Abort("CompositeMeshTechnique::registerVertexBuffer: unable to register vertex buffer.");
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
  try
  {
    for (AbstractMeshTechnique* subtechnique : _subtechniques)
    {
      subtechnique->registerIndicesBuffer(topology,
                                          buffer,
                                          indexType,
                                          indicesNumber);
    }
  }
  catch (std::exception& error)
  {
    Log() << error.what();
    Abort("CompositeMeshTechnique::registerIndicesBuffer: unable to register index buffer.");
  }
  catch(...)
  {
    Abort("CompositeMeshTechnique::registerIndicesBuffer: unable to register index buffer.");
  }
}

void CompositeMeshTechnique::unregisterIndicesBuffer(
                                          VkPrimitiveTopology topology) noexcept
{
  for (AbstractMeshTechnique* subtechnique : _subtechniques)
  {
    subtechnique->unregisterIndicesBuffer(topology);
  }
}

void CompositeMeshTechnique::registerUniformBuffer( Buffer& buffer,
                                                    const std::string& name)
{
  try
  {
    for (AbstractMeshTechnique* subtechnique : _subtechniques)
    {
      subtechnique->registerUniformBuffer(buffer, name);
    }
  }
  catch (std::exception& error)
  {
    Log() << error.what();
    Abort("CompositeMeshTechnique::registerUniformBuffer: unable to register uniform buffer.");
  }
  catch(...)
  {
    Abort("CompositeMeshTechnique::registerUniformBuffer: unable to register uniform buffer.");
  }
}

void CompositeMeshTechnique::unregisterUniformBuffer(
                                              Buffer& buffer,
                                              const std::string& name) noexcept
{
  for (AbstractMeshTechnique* subtechnique : _subtechniques)
  {
    subtechnique->unregisterUniformBuffer(buffer, name);
  }
}

void CompositeMeshTechnique::registerSampler( Sampler& sampler,
                                              const std::string& name)
{
  try
  {
    for (AbstractMeshTechnique* subtechnique : _subtechniques)
    {
      subtechnique->registerSampler(sampler, name);
    }
  }
  catch (std::exception& error)
  {
    Log() << error.what();
    Abort("CompositeMeshTechnique::registerSampler: unable to register sampler.");
  }
  catch(...)
  {
    Abort("CompositeMeshTechnique::registerSampler: unable to register sampler.");
  }
}

void CompositeMeshTechnique::unregisterSampler(
                                              Sampler& sampler,
                                              const std::string& name) noexcept
{
  for (AbstractMeshTechnique* subtechnique : _subtechniques)
  {
    subtechnique->unregisterSampler(sampler, name);
  }
}

void CompositeMeshTechnique::registerVariable(AbstractMeshVariable& variable,
                                              const std::string& name)
{
  try
  {
    for (AbstractMeshTechnique* subtechnique : _subtechniques)
    {
      subtechnique->registerVariable(variable, name);
    }
  }
  catch (std::exception& error)
  {
    Log() << error.what();
    Abort("CompositeMeshTechnique::registerVariable: unable to register variable.");
  }
  catch(...)
  {
    Abort("CompositeMeshTechnique::registerVariable: unable to register variable.");
  }
}

void CompositeMeshTechnique::unregisterVariable
                                              (AbstractMeshVariable& variable,
                                              const std::string& name) noexcept
{
  for (AbstractMeshTechnique* subtechnique : _subtechniques)
  {
    subtechnique->unregisterVariable(variable, name);
  }
}
