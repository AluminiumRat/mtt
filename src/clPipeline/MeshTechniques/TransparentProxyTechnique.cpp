#include <mtt/clPipeline/MeshTechniques/TransparentProxyTechnique.h>
#include <mtt/clPipeline/MeshTechniques/UnlightedColorTechnique.h>

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
  return std::make_unique<UnlightedColorTechnique>(
                                                true,
                                                true,
                                                VK_COMPARE_OP_GREATER_OR_EQUAL,
                                                _topology);
}
