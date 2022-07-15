#include <mtt/particles/Drawable/ParticlesColorTechnique.h>
#include <mtt/particles/Drawable/ParticlesProxyTechnique.h>

using namespace mtt;

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

std::unique_ptr<AbstractMeshTechnique>
              ParticlesProxyTechnique::createTechnique(AreaModificatorSet& set)
{
  return std::make_unique<ParticlesColorTechnique>(_commonData, set);
}
