#include <mtt/clPipeline/MeshTechniques/ModelCompositeColorTechnique.h>
#include <mtt/clPipeline/MeshTechniques/ModelTechniquesFactory.h>
#include <mtt/clPipeline/MeshTechniques/ShadowmapMeshTechnique.h>
#include <mtt/clPipeline/constants.h>
#include <mtt/render/Mesh/DepthMeshTechnique.h>
#include <mtt/render/Mesh/Mesh.h>
#include <mtt/render/Mesh/UidMeshTechnique.h>

using namespace mtt;
using namespace mtt::clPipeline;

ModelTechniquesFactory::ModelTechniquesFactory(
                                            bool createColorTechnique,
                                            bool enableSelectionColorTechnique,
                                            bool createShadowmapTechnique,
                                            bool createDepthTechnique,
                                            bool createUidTechnique,
                                            VkPrimitiveTopology topology) :
  _createColorTechnique(createColorTechnique),
  _enableSelectionColorTechnique(enableSelectionColorTechnique),
  _createShadowmapTechnique(createShadowmapTechnique),
  _createDepthTechnique(createDepthTechnique),
  _createUidTechnique(createUidTechnique),
  _topology(topology)
{
}

void ModelTechniquesFactory::setupTechniques(Mesh& mesh)
{
  if (_createColorTechnique)
  {
    mesh.setTechnique(colorFrameType,
                      std::make_unique<ModelCompositeColorTechnique>(
                                                _enableSelectionColorTechnique,
                                                _topology));
  }

  if (_createShadowmapTechnique)
  {
    mesh.setTechnique(opaqueShadowmapFrameType,
                      std::make_unique<ShadowmapMeshTechnique>(_topology));
  }

  if (_createDepthTechnique)
  {
    mesh.setTechnique(depthmapFrameType,
                      std::make_unique<DepthMeshTechnique>(_topology));
  }

  if (_createUidTechnique)
  {
    mesh.setTechnique(uidFrameType,
                      std::make_unique<UidMeshTechnique>( _topology,
                                                          true,
                                                          true));
  }
}
