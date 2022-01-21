#include <algorithm>

#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/Mesh/Mesh.h>
#include <mtt/utilities/Abort.h>

using namespace mtt;

Mesh::Mesh(LogicalDevice& device) noexcept:
  _device(device),
  _verticesNumber(0),
  _extraData(device)
{
}

void Mesh::setVerticesNumber(uint32_t newValue) noexcept
{
  if(_verticesNumber == newValue) return;

  try
  {
    _techniquesList.pass(
      [&](AbstractMeshTechnique& technique)
      {
        technique.setVerticesNumber(newValue);
      });
  }
  catch (...)
  {
    Abort("Mesh::setVerticesNumber: unable to set vertices number to technique.");
  }

  _verticesNumber = newValue;
}

void Mesh::addVerticesBuffer( std::shared_ptr<Buffer> buffer,
                              const std::string& name)
{
  Buffers::iterator iBuffer = std::find_if( _vertexBuffers.begin(),
                                            _vertexBuffers.end(),
                                            [&](const BufferRecord& record)
                                                                        -> bool
                                            {
                                              return record.name == name;
                                            });
  if(iBuffer != _vertexBuffers.end()) Abort("Mesh::addVerticesBuffer: buffer with this name is already registered.");

  try
  {
    _techniquesList.pass(
      [&](AbstractMeshTechnique& technique)
      {
        technique.registerVertexBuffer(*buffer, name);
      });
  }
  catch(...)
  {
    Abort("Mesh::addVerticesBuffer: unable to register vertex buffer in technique.");
  }

  BufferRecord newRecord;
  newRecord.name = name;
  newRecord.buffer = buffer;
  _vertexBuffers.push_back(newRecord);
}

void Mesh::removeVerticesBuffer(size_t index) noexcept
{
  BufferRecord& record = _vertexBuffers[index];

  try
  {
    _techniquesList.pass(
      [&](AbstractMeshTechnique& technique)
      {
        technique.unregisterVertexBuffer(*record.buffer, record.name);
      });
  }
  catch(...)
  {
    Abort("Mesh::removeVerticesBuffer: unable to unregister vertex buffer in technique.");
  }

  _vertexBuffers.erase(_vertexBuffers.begin() + index);
}

void Mesh::removeVerticesBuffer(const std::string& name) noexcept
{
  for(size_t iBuffer = 0; iBuffer < _vertexBuffers.size(); iBuffer++)
  {
    if(_vertexBuffers[iBuffer].name == name)
    {
      removeVerticesBuffer(iBuffer);
      break;
    }
  }
}

void Mesh::setIndices(VkPrimitiveTopology topology,
                      std::shared_ptr<Buffer> buffer,
                      VkIndexType indexType,
                      size_t indicesNumber)
{
  if(buffer == nullptr) Abort("Mesh::setIndices: buffer is null");
  if (indicesNumber == 0) Abort("Mesh::setIndices: indicesNumber is null");

  removeIndices(topology);

  try
  {
    _techniquesList.pass(
      [&](AbstractMeshTechnique& technique)
      {
        technique.registerIndicesBuffer(topology,
                                        *buffer,
                                        indexType,
                                        indicesNumber);
      });
  }
  catch (...)
  {
    Abort("Mesh::addVerticesBuffer: unable to register indices in technique.");
  }

  _indicesTable[topology].buffer = buffer;
  _indicesTable[topology].indexType = indexType;
  _indicesTable[topology].indicesNumber = indicesNumber;
}

void Mesh::removeIndices(VkPrimitiveTopology topology) noexcept
{
  if (_indicesTable[topology].buffer == nullptr) return;

  try
  {
    _techniquesList.pass( [&](AbstractMeshTechnique& technique)
                          {
                            technique.unregisterIndicesBuffer(topology);
                          });
  }
  catch(...)
  {
    Abort("Mesh::removeIndices: unable to unregister indices from technique.");
  }

  _indicesTable[topology].buffer.reset();
  _indicesTable[topology].indicesNumber = 0;
}

void Mesh::setGeometry(const CommonMeshGeometry& meshData)
{
  if(!meshData.positions.empty())
  {
    std::shared_ptr<Buffer> positionsBuffer(new Buffer( _device,
                                                        Buffer::VERTEX_BUFFER));
    positionsBuffer->setData( meshData.positions.data(),
                              meshData.positions.size() * sizeof(glm::vec3));
    setPositionBuffer(positionsBuffer);
  }
  else removePositionBuffer();

  if(!meshData.normals.empty())
  {
    std::shared_ptr<Buffer> normalsBuffer(new Buffer( _device,
                                                      Buffer::VERTEX_BUFFER));
    normalsBuffer->setData( meshData.normals.data(),
                            meshData.normals.size() * sizeof(glm::vec3));
    setNormalBuffer(normalsBuffer);
  }
  else removeNormalBuffer();

  if(!meshData.tangents.empty())
  {
    std::shared_ptr<Buffer> tangentBuffer(new Buffer( _device,
                                                      Buffer::VERTEX_BUFFER));
    tangentBuffer->setData( meshData.tangents.data(),
                            meshData.tangents.size() * sizeof(glm::vec3));
    setTangentBuffer(tangentBuffer);
  }
  else removeTangentBuffer();

  if(!meshData.binormals.empty())
  {
    std::shared_ptr<Buffer> binormalBuffer( new Buffer( _device,
                                                        Buffer::VERTEX_BUFFER));
    binormalBuffer->setData(meshData.binormals.data(),
                            meshData.binormals.size() * sizeof(glm::vec3));
    setBinormalBuffer(binormalBuffer);
  }
  else removeBinormalBuffer();

  if(!meshData.texCoords.empty())
  {
    std::shared_ptr<Buffer> texCoordBuffer( new Buffer( _device,
                                                        Buffer::VERTEX_BUFFER));
    texCoordBuffer->setData(meshData.texCoords.data(),
                            meshData.texCoords.size() * sizeof(glm::vec2));
    setTexCoordBuffer(texCoordBuffer);
  }
  else removeTexCoordBuffer();

  if(!meshData.skeletonRefs.empty())
  {
    std::vector<uint32_t> indices;
    indices.reserve(meshData.skeletonRefs.size() *
                                      CommonMeshGeometry::MAX_BONES_PER_VERTEX);
    std::vector<float> weights;
    weights.reserve(meshData.skeletonRefs.size() *
                                      CommonMeshGeometry::MAX_BONES_PER_VERTEX);
    for(const CommonMeshGeometry::SkeletonRef& ref : meshData.skeletonRefs)
    {
      uint16_t iBone = 0;
      for(; iBone < ref.bonesNumber; iBone++)
      {
        indices.push_back(ref.bones[iBone].boneIndex);
        weights.push_back(ref.bones[iBone].weight);
      }
      for (; iBone < CommonMeshGeometry::MAX_BONES_PER_VERTEX; iBone++)
      {
        indices.push_back(0);
        weights.push_back(0);
      }
    }
    std::shared_ptr<Buffer> boneIndicesBuffer(
                                    new Buffer(_device, Buffer::VERTEX_BUFFER));
    boneIndicesBuffer->setData( indices.data(),
                                indices.size() * sizeof(uint32_t));
    setBoneIndecesBuffer(boneIndicesBuffer);

    std::shared_ptr<Buffer> boneWeightsBuffer(
                                    new Buffer(_device, Buffer::VERTEX_BUFFER));
    boneWeightsBuffer->setData( weights.data(),
                                weights.size() * sizeof(float));
    setBoneWeightsBuffer(boneWeightsBuffer);
  }
  else
  {
    removeBoneIndicesBuffer();
    removeBoneWeightsBuffer();
  }

  setVerticesNumber(uint32_t(meshData.positions.size()));

  if(!meshData.triangleIndices.empty())
  {
    std::shared_ptr<Buffer> indicesBuffer(
                                  new Buffer(_device, Buffer::INDICES_BUFFER));
    indicesBuffer->setData( meshData.triangleIndices.data(),
                            meshData.triangleIndices.size() * sizeof(uint16_t));
    setIndices( VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
                indicesBuffer,
                VK_INDEX_TYPE_UINT16,
                meshData.triangleIndices.size());
  }
  else removeIndices(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

  if(!meshData.lineIndices.empty())
  {
    std::shared_ptr<Buffer> indicesBuffer(
                                  new Buffer(_device, Buffer::INDICES_BUFFER));
    indicesBuffer->setData( meshData.lineIndices.data(),
                            meshData.lineIndices.size() * sizeof(uint16_t));
    setIndices( VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
                indicesBuffer,
                VK_INDEX_TYPE_UINT16,
                meshData.lineIndices.size());
  }
  else removeIndices(VK_PRIMITIVE_TOPOLOGY_LINE_LIST);
}

void Mesh::_releaseTechnique(AbstractMeshTechnique& technique) noexcept
{
  try
  {
    technique.setMeshExtraData(nullptr);
    for(BufferRecord& bufferRecord : _vertexBuffers)
    {
      technique.unregisterVertexBuffer(*bufferRecord.buffer, bufferRecord.name);
    }
  
    for(size_t topology = 0; topology < topologiesNumber; topology++)
    {
      if(_indicesTable[topology].buffer != nullptr)
      {
        technique.unregisterIndicesBuffer(VkPrimitiveTopology(topology));
      }
    }
  }
  catch(...)
  {
    Abort("Mesh::_releaseTechnique: Unable to release technique");
  }
}

void Mesh::setTechnique(FrameType frameType,
                        std::unique_ptr<AbstractMeshTechnique> newTechnique)
{
  AbstractMeshTechnique* oldTechnique = _techniquesList.get(frameType);
  if(oldTechnique != nullptr) _releaseTechnique(*oldTechnique);

  try
  {
    newTechnique->setVerticesNumber(_verticesNumber);
    newTechnique->setMeshExtraData(&_extraData);
    for(BufferRecord& bufferRecord : _vertexBuffers)
    {
      newTechnique->registerVertexBuffer( *bufferRecord.buffer,
                                          bufferRecord.name);
    }
  
    for(size_t topology = 0; topology < topologiesNumber; topology++)
    {
      if (_indicesTable[topology].buffer != nullptr)
      {
        newTechnique->registerIndicesBuffer(
                                        VkPrimitiveTopology(topology),
                                        *_indicesTable[topology].buffer,
                                        _indicesTable[topology].indexType,
                                        _indicesTable[topology].indicesNumber);
      }
    }
  }
  catch(...)
  {
    Abort("Mesh::setTechnique: Unable to update technique");
  }

  _techniquesList.set(std::move(newTechnique), frameType);
}

std::unique_ptr<AbstractMeshTechnique> Mesh::removeTechnique(
                                                  FrameType frameType) noexcept
{
  std::unique_ptr<AbstractMeshTechnique> removedTechnique =
                                            _techniquesList.remove(frameType);
  if(removedTechnique != nullptr) _releaseTechnique(*removedTechnique);
  return removedTechnique;
}

void Mesh::buildDrawActions(DrawPlanBuildInfo& buildInfo)
{
  AbstractMeshTechnique* technique = _techniquesList.get(buildInfo.frameType);
  if(technique != nullptr) technique->addToDrawPlan(buildInfo);
}
