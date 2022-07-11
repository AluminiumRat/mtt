#include <mtt/clPipeline/Particles/ParticlesColorTechnique.h>
#include <mtt/clPipeline/Particles/ParticlesProxyTechnique.h>

using namespace mtt;
using namespace mtt::clPipeline;

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
