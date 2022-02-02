#include <mtt/clPipeline/MeshTechniques/TransparentProxyTechnique.h>
#include <mtt/clPipeline/MeshTechniques/FLColorTechnique.h>

using namespace mtt;
using namespace clPipeline;

TransparentProxyTechnique::TransparentProxyTechnique(
                                                VkPrimitiveTopology topology) :
  _topology(topology)
{

}

std::unique_ptr<AbstractMeshTechnique>
            TransparentProxyTechnique::createTechnique(AreaModificatorSet& set)
{
  return std::make_unique<FLColorTechnique>(set, _topology);
}
