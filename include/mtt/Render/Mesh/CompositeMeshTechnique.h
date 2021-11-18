#pragma once

#include <vector>

#include <mtt/Render/Mesh/AbstractMeshTechnique.h>

namespace mtt
{
  class CompositeMeshTechnique : public AbstractMeshTechnique
  {
  public:
    CompositeMeshTechnique() = default;
    CompositeMeshTechnique(const CompositeMeshTechnique&) = delete;
    CompositeMeshTechnique& operator = (const CompositeMeshTechnique&) = delete;
    virtual ~CompositeMeshTechnique() noexcept = default;

    virtual void setVerticesNumber(uint32_t newValue) override;
    virtual void setMeshExtraData(MeshExtraData* newData) override;
    virtual void registerVertexBuffer(Buffer& buffer,
                                      const std::string& name) override;
    virtual void unregisterVertexBuffer(
                                    Buffer& buffer,
                                    const std::string& name) noexcept override;

    virtual void registerIndicesBuffer( VkPrimitiveTopology topology,
                                        Buffer& buffer,
                                        VkIndexType indexType,
                                        size_t indicesNumber) override;
    virtual void unregisterIndicesBuffer(VkPrimitiveTopology topology) override;

    virtual void registerUniformBuffer( Buffer& buffer,
                                        const std::string& name) override;
    virtual void unregisterUniformBuffer(
                                    Buffer& buffer,
                                    const std::string& name) noexcept override;

    virtual void registerSampler( Sampler& sampler,
                                  const std::string& name) override;
    virtual void unregisterSampler( Sampler& sampler,
                                    const std::string& name) noexcept override;

    virtual void registerVariable(AbstractMeshVariable& variable,
                                  const std::string& name) override;
    virtual void unregisterVariable(AbstractMeshVariable& variable,
                                    const std::string& name) noexcept override;

  protected:
    /// call from constructor
    void registerSubtechnique(AbstractMeshTechnique& technique);

  private:
    using Subtechniques = std::vector<AbstractMeshTechnique*>;
    Subtechniques _subtechniques;
  };
}