#include <mtt/DLPipeline/MeshTechniques/MeshCompositeColorTechnique.h>
#include <mtt/DLPipeline/MeshTechniques/ModelTechniquesFactory.h>
#include <mtt/DLPipeline/MeshTechniques/ShadowmapMeshTechnique.h>
#include <mtt/DLPipeline/constants.h>
#include <mtt/Render/Mesh/DepthMeshTechnique.h>
#include <mtt/Render/Mesh/Mesh.h>
#include <mtt/Render/Mesh/UidMeshTechnique.h>

using namespace mtt;
using namespace mtt::DLPipeline;

ModelTechniquesFactory::ModelTechniquesFactory( bool createColorTechnique,
                                                bool createShadowmapTechnique,
                                                bool createDepthTechnique,
                                                bool createUidTechnique,
                                                VkPrimitiveTopology topology) :
  _createColorTechnique(createColorTechnique),
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
                      std::make_unique<MeshCompositeColorTechnique>(_topology));
  }

  if (_createShadowmapTechnique)
  {
    mesh.setTechnique(shadowmapFrameType,
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
