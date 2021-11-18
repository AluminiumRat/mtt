#pragma once

#include <string>

#include <vulkan/vulkan.h>

#include <mtt/Render/Mesh/MeshVariable.h>

namespace mtt
{
  class Buffer;
  struct DrawPlanBuildInfo;
  class MeshExtraData;
  class Sampler;

  class AbstractMeshTechnique
  {
  public:
    AbstractMeshTechnique();
    AbstractMeshTechnique(const AbstractMeshTechnique&) = delete;
    AbstractMeshTechnique& operator = (const AbstractMeshTechnique&) = delete;
    virtual ~AbstractMeshTechnique() noexcept;

    virtual void setVerticesNumber(uint32_t newValue);
    inline uint32_t verticesNumber() const noexcept;

    virtual void setMeshExtraData(MeshExtraData* newData);
    inline MeshExtraData* meshExtraData() const noexcept;

    virtual void registerVertexBuffer(Buffer& buffer,
                                      const std::string& name) = 0;
    virtual void unregisterVertexBuffer(Buffer& buffer,
                                        const std::string& name)  noexcept = 0;

    virtual void registerIndicesBuffer( VkPrimitiveTopology topology,
                                        Buffer& buffer,
                                        VkIndexType indexType,
                                        size_t indicesNumber) = 0;
    virtual void unregisterIndicesBuffer(VkPrimitiveTopology topology) = 0;

    virtual void registerUniformBuffer( Buffer& buffer,
                                        const std::string& name) = 0;
    virtual void unregisterUniformBuffer( Buffer& buffer,
                                          const std::string& name)  noexcept = 0;

    virtual void registerSampler( Sampler& sampler,
                                  const std::string& name) = 0;
    virtual void unregisterSampler( Sampler& sampler,
                                    const std::string& name)  noexcept = 0;

    virtual void registerVariable(AbstractMeshVariable& variable,
                                  const std::string& name) = 0;
    virtual void unregisterVariable(AbstractMeshVariable& variable,
                                    const std::string& name) noexcept = 0;

    virtual void addToDrawPlan(DrawPlanBuildInfo& buildInfo) = 0;

  private:
    uint32_t _verticesNumber;
    MeshExtraData* _meshExtraData;
  };

  inline uint32_t AbstractMeshTechnique::verticesNumber() const noexcept
  {
    return _verticesNumber;
  }

  inline MeshExtraData* AbstractMeshTechnique::meshExtraData() const noexcept
  {
    return _meshExtraData;
  }
}