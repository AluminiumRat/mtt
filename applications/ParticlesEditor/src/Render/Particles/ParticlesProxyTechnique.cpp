#include <Render/Particles/ParticlesColorTechnique.h>
#include <Render/Particles/ParticlesProxyTechnique.h>

ParticlesProxyTechnique::ParticlesProxyTechnique(
                                          ParticlesDrawCommonData& commonData) :
  _commonData(commonData)
{
}

void ParticlesProxyTechnique::clearPipelines() noexcept
{
  for(size_t techniqueIndex = 0;
      techniqueIndex < subtechniquesNumber();
      techniqueIndex++)
  {
    ParticlesColorTechnique& technqiue =
            static_cast<ParticlesColorTechnique&>(subtechnique(techniqueIndex));
    technqiue.clearPipeline();
  }
}

std::unique_ptr<mtt::AbstractMeshTechnique>
          ParticlesProxyTechnique::createTechnique(
                                                  mtt::AreaModificatorSet& set)
{
  return std::make_unique<ParticlesColorTechnique>(_commonData, set);
}
