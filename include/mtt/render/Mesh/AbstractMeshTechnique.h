#pragma once

#include <string>

#include <vulkan/vulkan.h>

#include <mtt/render/Mesh/MeshVariable.h>

namespace mtt
{
  class AreaModificatorSet;
  class Buffer;
  class CompositeMeshTechnique;
  struct DrawPlanBuildInfo;
  class Mesh;
  class Sampler;

  class AbstractMeshTechnique
  {
  public:
    AbstractMeshTechnique();
    AbstractMeshTechnique(const AbstractMeshTechnique&) = delete;
    AbstractMeshTechnique& operator = (const AbstractMeshTechnique&) = delete;
    virtual ~AbstractMeshTechnique() noexcept;

    inline Mesh* mesh() const noexcept;

    virtual void setVerticesNumber(uint32_t newValue);
    inline uint32_t verticesNumber() const noexcept;

    virtual void registerVertexBuffer(Buffer& buffer,
                                      const std::string& name);
    virtual void unregisterVertexBuffer(Buffer& buffer,
                                        const std::string& name)  noexcept;

    virtual void registerIndicesBuffer( VkPrimitiveTopology topology,
                                        Buffer& buffer,
                                        VkIndexType indexType,
                                        size_t indicesNumber);
    virtual void unregisterIndicesBuffer(VkPrimitiveTopology topology) noexcept;

    virtual void registerUniformBuffer( Buffer& buffer,
                                        const std::string& name);
    virtual void unregisterUniformBuffer( Buffer& buffer,
                                          const std::string& name)  noexcept;

    virtual void registerSampler( Sampler& sampler,
                                  const std::string& name);
    virtual void unregisterSampler( Sampler& sampler,
                                    const std::string& name)  noexcept;

    virtual void registerVariable(AbstractMeshVariable& variable,
                                  const std::string& name);
    virtual void unregisterVariable(AbstractMeshVariable& variable,
                                    const std::string& name) noexcept;

    virtual void registerAreaModificators(AreaModificatorSet& set);
    virtual void unregisterAreaModificators(AreaModificatorSet& set) noexcept;

    virtual void addToDrawPlan(DrawPlanBuildInfo& buildInfo) = 0;

  protected:
    friend class CompositeMeshTechnique;
    friend class Mesh;
    virtual void setMesh(Mesh* newMesh);

  private:
    Mesh* _mesh;
    uint32_t _verticesNumber;
  };

  inline Mesh* AbstractMeshTechnique::mesh() const noexcept
  {
    return _mesh;
  }

  inline uint32_t AbstractMeshTechnique::verticesNumber() const noexcept
  {
    return _verticesNumber;
  }
}