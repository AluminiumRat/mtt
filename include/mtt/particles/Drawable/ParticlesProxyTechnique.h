#pragma once

#include <mtt/render/Drawable/ModificatorProxyDrawable.h>

namespace mtt
{
  class ParticlesDrawCommonData;

  class ParticlesProxyTechnique : public ModificatorProxyDrawable
  {
  public:
    ParticlesProxyTechnique(ParticlesDrawCommonData& commonData);
    ParticlesProxyTechnique(const ParticlesProxyTechnique&) = delete;
    ParticlesProxyTechnique& operator = (
                                      const ParticlesProxyTechnique&) = delete;
    virtual ~ParticlesProxyTechnique() noexcept = default;

    void resetPipelines() noexcept;

  protected:
    virtual std::unique_ptr<Drawable>
                            createSubdrawable(AreaModificatorSet& set) override;

  private:
    ParticlesDrawCommonData& _commonData;
  };
}