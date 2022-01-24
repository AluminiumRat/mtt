#pragma once

#include <mtt/render/Mesh/AbstractMeshTechnique.h>

namespace mtt
{
  class MonotopologyMeshTechnique : public AbstractMeshTechnique
  {
  public:
    explicit MonotopologyMeshTechnique(VkPrimitiveTopology topology);
    MonotopologyMeshTechnique(const MonotopologyMeshTechnique&) = delete;
    MonotopologyMeshTechnique& operator = (
                                    const MonotopologyMeshTechnique&) = delete;
    virtual ~MonotopologyMeshTechnique() noexcept = default;

    virtual void registerIndicesBuffer( VkPrimitiveTopology topology,
                                        Buffer& buffer,
                                        VkIndexType indexType,
                                        size_t indicesNumber) override;
    virtual void unregisterIndicesBuffer(
                                VkPrimitiveTopology topology) noexcept override;

    inline VkPrimitiveTopology topology() const noexcept;
    inline Buffer* indicesBuffer() const noexcept;
    inline VkIndexType indexType() const noexcept;
    inline size_t indicesNumber() const noexcept;

    inline uint32_t vertexCountToDraw() const noexcept;

  protected:
    virtual void setIndicesBuffer(Buffer* buffer,
                                  VkIndexType indexType) noexcept;

  private:
    VkPrimitiveTopology _topology;
    Buffer* _indicesBuffer;
    VkIndexType _indexType;
    size_t _indicesNumber;
  };

  inline VkPrimitiveTopology MonotopologyMeshTechnique::topology() const noexcept
  {
    return _topology;
  }

  inline Buffer* MonotopologyMeshTechnique::indicesBuffer() const noexcept
  {
    return _indicesBuffer;
  }

  inline VkIndexType MonotopologyMeshTechnique::indexType() const noexcept
  {
    return _indexType;
  }

  inline size_t MonotopologyMeshTechnique::indicesNumber() const noexcept
  {
    return _indicesNumber;
  }

  inline uint32_t MonotopologyMeshTechnique::vertexCountToDraw() const noexcept
  {
    return uint32_t(indicesBuffer() != nullptr ?  indicesNumber() :
                                                  verticesNumber());
  }
}