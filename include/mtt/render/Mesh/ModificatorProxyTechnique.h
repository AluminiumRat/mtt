#pragma once

#include <memory>
#include <vector>

#include <mtt/render/Mesh/CompositeMeshTechnique.h>

namespace mtt
{
  class ModificatorProxyTechnique : public CompositeMeshTechnique
  {
  public:
    ModificatorProxyTechnique() = default;
    ModificatorProxyTechnique(const ModificatorProxyTechnique&) = delete;
    ModificatorProxyTechnique& operator =
                                    (const ModificatorProxyTechnique&) = delete;
    virtual ~ModificatorProxyTechnique() noexcept;

    virtual void registerAreaModificators(AreaModificatorSet& set) override;
    virtual void unregisterAreaModificators(
                                    AreaModificatorSet& set) noexcept override;

    virtual void addToDrawPlan(DrawPlanBuildInfo& buildInfo) override;

  protected:
    virtual std::unique_ptr<AbstractMeshTechnique>
                                  createTechnique(AreaModificatorSet& set) = 0;

  private:
    struct TechniqueRecord
    {
      AreaModificatorSet* set = nullptr;
      size_t registerCounter = 0;
      std::unique_ptr<AbstractMeshTechnique> technique;

      TechniqueRecord() = default;
      TechniqueRecord(const TechniqueRecord&) = delete;
      TechniqueRecord& operator = (const TechniqueRecord&) = delete;
      TechniqueRecord(TechniqueRecord&&) = default;
      TechniqueRecord& operator = (TechniqueRecord&&) = default;
      ~TechniqueRecord() noexcept = default;
    };
    using Subtechniques = std::vector<TechniqueRecord>;
    Subtechniques _subtechniques;
  };
}