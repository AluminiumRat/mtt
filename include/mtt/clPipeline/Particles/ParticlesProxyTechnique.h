#pragma once

#include <mtt/clPipeline/MeshTechniques/TransparentProxyTechnique.h>

namespace mtt
{
  namespace clPipeline
  {

    class ParticlesDrawCommonData;

    class ParticlesProxyTechnique : public ModificatorProxyTechnique
    {
    public:
      ParticlesProxyTechnique(ParticlesDrawCommonData& commonData);
      ParticlesProxyTechnique(const ParticlesProxyTechnique&) = delete;
      ParticlesProxyTechnique& operator = (
                                      const ParticlesProxyTechnique&) = delete;
      virtual ~ParticlesProxyTechnique() noexcept = default;

      void clearPipelines() noexcept;

    protected:
      virtual std::unique_ptr<AbstractMeshTechnique>
                              createTechnique(AreaModificatorSet& set) override;

    private:
      ParticlesDrawCommonData& _commonData;
    };
  }
}