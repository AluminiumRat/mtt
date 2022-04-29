#pragma once

#include <mtt/clPipeline/MeshTechniques/TransparentProxyTechnique.h>

class ParticlesDrawCommonData;

class ParticlesProxyTechnique : public mtt::ModificatorProxyTechnique
{
public:
  ParticlesProxyTechnique(ParticlesDrawCommonData& commonData);
  ParticlesProxyTechnique(const ParticlesProxyTechnique&) = delete;
  ParticlesProxyTechnique& operator = (const ParticlesProxyTechnique&) = delete;
  virtual ~ParticlesProxyTechnique() noexcept = default;

  void clearPipelines() noexcept;

protected:
  virtual std::unique_ptr<mtt::AbstractMeshTechnique>
                        createTechnique(mtt::AreaModificatorSet& set) override;

private:
  ParticlesDrawCommonData& _commonData;
};
