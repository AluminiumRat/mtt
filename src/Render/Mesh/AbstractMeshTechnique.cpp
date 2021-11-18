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
