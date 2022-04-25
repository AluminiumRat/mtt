#pragma once

#include <vector>

#include <mtt/render/Mesh/AbstractMeshTechnique.h>

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
    virtual void registerVertexBuffer(Buffer& buffer,
                                      const std::string& name) override;
    virtual void unregisterVertexBuffer(
                                    Buffer& buffer,
                                    const std::string& name) noexcept override;

    virtual void registerIndicesBuffer( VkPrimitiveTopology topology,
                                        Buffer& buffer,
                                        VkIndexType indexType,
                                        size_t indicesNumber) override;
    virtual void unregisterIndicesBuffer(
                                VkPrimitiveTopology topology) noexcept override;

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

    virtual void registerAreaModificators(AreaModificatorSet& set) override;
    virtual void unregisterAreaModificators(
                                    AreaModificatorSet& set) noexcept override;

  protected:
    void registerSubtechnique(AbstractMeshTechnique& technique);
    void unregisterSubtechnique(AbstractMeshTechnique& technique);

    inline size_t subtechniquesNumber() const noexcept;
    inline const AbstractMeshTechnique& subtechnique(
                                                  size_t index) const noexcept;
    inline AbstractMeshTechnique& subtechnique(size_t index) noexcept;

    virtual void setMesh(Mesh* newMesh) override;

  private:
    using Subtechniques = std::vector<AbstractMeshTechnique*>;
    Subtechniques _subtechniques;
  };

  inline size_t CompositeMeshTechnique::subtechniquesNumber() const noexcept
  {
    return _subtechniques.size();
  }

  inline const AbstractMeshTechnique& CompositeMeshTechnique::subtechnique(
                                                    size_t index) const noexcept
  {
    return *_subtechniques[index];
  }

  inline AbstractMeshTechnique& CompositeMeshTechnique::subtechnique(
                                                          size_t index) noexcept
  {
    return *_subtechniques[index];
  }
}