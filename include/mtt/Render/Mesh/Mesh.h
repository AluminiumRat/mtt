#pragma once

#include <memory>
#include <string>

#include <vulkan/vulkan.h>

#include <mtt/Render/Drawable/Drawable.h>
#include <mtt/Render/Mesh/AbstractMeshTechnique.h>
#include <mtt/Render/Mesh/MeshExtraData.h>
#include <mtt/Render/Mesh/TechniquesCollection.h>
#include <mtt/Render/Pipeline/Buffer.h>
#include <mtt/Render/Pipeline/VertexAttribute.h>
#include <mtt/Render/SceneRenderer/AbstractFrameBuilder.h>
#include <mtt/Render/SceneRenderer/FrameType.h>
#include <mtt/Utilities/CommonMeshGeometry.h>

namespace mtt
{
  struct DrawPlanBuildInfo;
  class LogicalDevice;

  class Mesh : public Drawable
  {
  public:
    inline static const char positionBufferName[] = "position";
    inline static const char positionAttributeLocation[] = "positionLocation";
    inline static const VertexAttribute::DataType positionAttributeType =
                                              VertexAttribute::FLOAT_VEC3_TYPE;

    inline static const char normalBufferName[] = "normal";
    inline static const char normalAttributeLocation[] = "normalLocation";
    inline static const VertexAttribute::DataType normalAttributeType =
                                              VertexAttribute::FLOAT_VEC3_TYPE;

    inline static const char tangentBufferName[] = "tangent";
    inline static const char tangentAttributeLocation[] = "tangentLocation";
    inline static const VertexAttribute::DataType tangentAttributeType =
                                              VertexAttribute::FLOAT_VEC3_TYPE;

    inline static const char binormalBufferName[] = "binormal";
    inline static const char binormalAttributeLocation[] = "binormalLocation";
    inline static const VertexAttribute::DataType binormalAttributeType =
                                              VertexAttribute::FLOAT_VEC3_TYPE;

    inline static const char texCoordBufferName[] = "texCoord";
    inline static const char texCoordAttributeLocation[] = "texCoordLocation";
    inline static const VertexAttribute::DataType texCoordAttributeType =
                                              VertexAttribute::FLOAT_VEC2_TYPE;

    inline static const char boneIndicesBufferName[] = "boneIndices";
    inline static const char boneIndicesAttributeLocation[] =
                                                          "boneIndicesLocation";
    inline static const VertexAttribute::DataType boneIndicesAttributeType =
                                              VertexAttribute::UINT32_VEC4_TYPE;

    inline static const char boneWeightsBufferName[] = "boneWeights";
    inline static const char boneWeightsAttributeLocation[] =
                                                          "boneWeightsLocation";
    inline static const VertexAttribute::DataType boneWeightsAttributeType =
                                              VertexAttribute::FLOAT_VEC4_TYPE;

  public:
    struct IndicesRecord
    {
      std::shared_ptr<Buffer> buffer;
      VkIndexType indexType = VK_INDEX_TYPE_UINT16;
      size_t indicesNumber = 0;
    };

  public:
    explicit Mesh(LogicalDevice& device) noexcept;
    Mesh(const Mesh&) = delete;
    Mesh& operator = (const Mesh&) = delete;
    virtual ~Mesh() noexcept = default;

    inline LogicalDevice& device() const noexcept;

    inline uint32_t verticesNumber() const noexcept;
    void setVerticesNumber(uint32_t newValue) noexcept;

    inline size_t verticesBuffersNumber() const noexcept;
    inline Buffer& verticesBuffer(size_t index) noexcept;
    inline const Buffer& verticesBuffer(size_t index) const noexcept;
    inline const std::string& verticesBufferName(size_t index) const noexcept;
    void addVerticesBuffer( std::shared_ptr<Buffer> buffer,
                            const std::string& name);
    void removeVerticesBuffer(size_t index) noexcept;
    void removeVerticesBuffer(const std::string& name) noexcept;

    inline void setPositionBuffer(std::shared_ptr<Buffer> buffer);
    inline void removePositionBuffer() noexcept;

    inline void setNormalBuffer(std::shared_ptr<Buffer> buffer);
    inline void removeNormalBuffer() noexcept;

    inline void setTangentBuffer(std::shared_ptr<Buffer> buffer);
    inline void removeTangentBuffer() noexcept;

    inline void setBinormalBuffer(std::shared_ptr<Buffer> buffer);
    inline void removeBinormalBuffer() noexcept;

    inline void setTexCoordBuffer(std::shared_ptr<Buffer> buffer);
    inline void removeTexCoordBuffer() noexcept;

    inline void setBoneIndecesBuffer(std::shared_ptr<Buffer> buffer);
    inline void removeBoneIndicesBuffer() noexcept;

    inline void setBoneWeightsBuffer(std::shared_ptr<Buffer> buffer);
    inline void removeBoneWeightsBuffer() noexcept;

    inline const IndicesRecord& indices(
                                  VkPrimitiveTopology topology) const noexcept;
    /// buffer must not be nullptr, indicesNumber should not be null.
    void setIndices(VkPrimitiveTopology topology,
                    std::shared_ptr<Buffer> buffer,
                    VkIndexType indexType,
                    size_t indicesNumber);
    void removeIndices(VkPrimitiveTopology topology) noexcept;

    void setGeometry(const CommonMeshGeometry& meshData);

    inline MeshExtraData& extraData() noexcept;
    inline const MeshExtraData& extraData() const noexcept;

    inline AbstractMeshTechnique* technique(FrameType frameType) noexcept;
    inline const AbstractMeshTechnique* technique(
                                            FrameType frameType) const noexcept;
    void setTechnique(FrameType frameType,
                      std::unique_ptr<AbstractMeshTechnique> newTechnique);
    /// Returns the removed technique
    std::unique_ptr<AbstractMeshTechnique> removeTechnique(
                                                  FrameType frameType) noexcept;

  protected:
    virtual void buildDrawActions(DrawPlanBuildInfo& buildInfo) override;

  private:
    void _releaseTechnique(AbstractMeshTechnique& technique) noexcept;

  private:
    LogicalDevice& _device;

    uint32_t _verticesNumber;

    struct BufferRecord
    {
      std::string name;
      std::shared_ptr<Buffer> buffer;
    };
    using Buffers = std::vector<BufferRecord>;
    Buffers _vertexBuffers;

    MeshExtraData _extraData;

    TechniquesCollection<AbstractMeshTechnique> _techniquesList;

    inline static const size_t topologiesNumber =
                                      size_t(VK_PRIMITIVE_TOPOLOGY_PATCH_LIST);
    IndicesRecord _indicesTable[topologiesNumber];
  };

  inline LogicalDevice& Mesh::device() const noexcept
  {
    return _device;
  }

  inline uint32_t Mesh::verticesNumber() const noexcept
  {
    return _verticesNumber;
  }

  inline size_t Mesh::verticesBuffersNumber() const noexcept
  {
    return _vertexBuffers.size();
  }

  inline Buffer& Mesh::verticesBuffer(size_t index) noexcept
  {
    return *_vertexBuffers[index].buffer;
  }

  inline const Buffer& Mesh::verticesBuffer(size_t index) const noexcept
  {
    return *_vertexBuffers[index].buffer;
  }

  inline const std::string& Mesh::verticesBufferName(
                                                  size_t index) const noexcept
  {
    return _vertexBuffers[index].name;
  }

  inline void Mesh::setPositionBuffer(std::shared_ptr<Buffer> buffer)
  {
    removePositionBuffer();
    addVerticesBuffer(std::move(buffer), positionBufferName);
  }

  inline void Mesh::removePositionBuffer() noexcept
  {
    removeVerticesBuffer(positionBufferName);
  }

  inline void Mesh::setNormalBuffer(std::shared_ptr<Buffer> buffer)
  {
    removeNormalBuffer();
    addVerticesBuffer(std::move(buffer), normalBufferName);
  }

  inline void Mesh::removeNormalBuffer() noexcept
  {
    removeVerticesBuffer(normalBufferName);
  }

  inline void Mesh::setTangentBuffer(std::shared_ptr<Buffer> buffer)
  {
    removeTangentBuffer();
    addVerticesBuffer(std::move(buffer), tangentBufferName);
  }

  inline void Mesh::removeTangentBuffer() noexcept
  {
    removeVerticesBuffer(tangentBufferName);
  }

  inline void Mesh::setBinormalBuffer(std::shared_ptr<Buffer> buffer)
  {
    removeBinormalBuffer();
    addVerticesBuffer(std::move(buffer), binormalBufferName);
  }

  inline void Mesh::removeBinormalBuffer() noexcept
  {
    removeVerticesBuffer(binormalBufferName);
  }

  inline void Mesh::setTexCoordBuffer(std::shared_ptr<Buffer> buffer)
  {
    removeTexCoordBuffer();
    addVerticesBuffer(std::move(buffer), texCoordBufferName);
  }

  inline void Mesh::removeTexCoordBuffer() noexcept
  {
    removeVerticesBuffer(texCoordBufferName);
  }

  inline void Mesh::setBoneIndecesBuffer(std::shared_ptr<Buffer> buffer)
  {
    removeBoneIndicesBuffer();
    addVerticesBuffer(std::move(buffer), boneIndicesBufferName);
  }

  inline void Mesh::removeBoneIndicesBuffer() noexcept
  {
    removeVerticesBuffer(boneIndicesBufferName);
  }

  inline void Mesh::setBoneWeightsBuffer(std::shared_ptr<Buffer> buffer)
  {
    removeBoneWeightsBuffer();
    addVerticesBuffer(std::move(buffer), boneWeightsBufferName);
  }

  inline void Mesh::removeBoneWeightsBuffer() noexcept
  {
    removeVerticesBuffer(boneWeightsBufferName);
  }

  inline const Mesh::IndicesRecord& Mesh::indices(
                                    VkPrimitiveTopology topology) const noexcept
  {
    return _indicesTable[size_t(topology)];
  }

  inline MeshExtraData& Mesh::extraData() noexcept
  {
    return _extraData;
  }

  inline const MeshExtraData& Mesh::extraData() const noexcept
  {
    return _extraData;
  }

  inline AbstractMeshTechnique* Mesh::technique(FrameType frameType) noexcept
  {
    return _techniquesList.get(frameType);
  }

  inline const AbstractMeshTechnique* Mesh::technique(
                                            FrameType frameType) const noexcept
  {
    return _techniquesList.get(frameType);
  }
}