#include <mtt/render/Mesh/MonotopologyMeshTechnique.h>

using namespace mtt;

MonotopologyMeshTechnique::MonotopologyMeshTechnique(
                                                  VkPrimitiveTopology topology):
  _topology(topology),
  _indicesBuffer(nullptr),
  _indexType(VK_INDEX_TYPE_UINT16),
  _indicesNumber(0)
{
}

void MonotopologyMeshTechnique::registerIndicesBuffer(
                                                  VkPrimitiveTopology topology,
                                                  Buffer& buffer,
                                                  VkIndexType indexType,
                                                  size_t indicesNumber)
{
  if(topology != _topology) return;
  _indicesBuffer = &buffer;
  _indexType = indexType;
  _indicesNumber = indicesNumber;
  setIndicesBuffer(_indicesBuffer, _indexType);
}

void MonotopologyMeshTechnique::unregisterIndicesBuffer(
                                          VkPrimitiveTopology topology) noexcept
{
  if(topology != _topology) return;
  if(_indicesBuffer == nullptr) return;
  _indicesBuffer = nullptr;
  _indicesNumber = 0;
  setIndicesBuffer(_indicesBuffer, _indexType);
}

void MonotopologyMeshTechnique::setIndicesBuffer(
                                                Buffer* buffer,
                                                VkIndexType indexType) noexcept
{
}
