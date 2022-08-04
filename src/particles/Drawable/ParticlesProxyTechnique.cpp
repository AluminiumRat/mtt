#include <mtt/particles/Drawable/ParticlesColorTechnique.h>
#include <mtt/particles/Drawable/ParticlesProxyTechnique.h>

using namespace mtt;

ParticlesProxyTechnique::ParticlesProxyTechnique(
                                          ParticlesDrawCommonData& commonData) :
  _commonData(commonData)
{
}

void ParticlesProxyTechnique::resetPipelines() noexcept
{
  for(size_t techniqueIndex = 0;
      techniqueIndex < childsNumber();
      techniqueIndex++)
  {
    ParticlesColorTechnique& technqiue =
            static_cast<ParticlesColorTechnique&>(child(techniqueIndex));
    technqiue.resetPipeline();
  }
}

std::unique_ptr<Drawable>
            ParticlesProxyTechnique::createSubdrawable(AreaModificatorSet& set)
{
  return std::make_unique<ParticlesColorTechnique>(_commonData, set);
}
