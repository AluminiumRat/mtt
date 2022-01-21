#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/dlPipeline/MeshTechniques/MeshCompositeColorTechnique.h>

using namespace mtt;
using namespace mtt::dlPipeline;

MeshCompositeColorTechnique::MeshCompositeColorTechnique(
                                                VkPrimitiveTopology topology) :
  _opaqueTechnique(topology),
  _opaqueTechniqueEnabled(false),
  _transparentTechnique(topology),
  _transparentTechniqueEnabled(false),
  _emissionTechnique(topology),
  _emissionTechniqueEnabled(false),
  _selectionTechnique(topology),
  _useAlphaFromAlbedoSampler(false),
  _opaqueTextureEnabled(false)
{
  registerSubtechnique(_opaqueTechnique);
  registerSubtechnique(_transparentTechnique);
  registerSubtechnique(_emissionTechnique);
  registerSubtechnique(_selectionTechnique);

  _updateTechniquesFlags();
}

void MeshCompositeColorTechnique::_updateTechniquesFlags() noexcept
{
  _opaqueTechniqueEnabled = false;
  _transparentTechniqueEnabled = false;
  _emissionTechniqueEnabled = false;

  if(!_materialData.has_value())
  {
    _opaqueTechniqueEnabled = true;
    return;
  }

  if( _materialData->opaque != 1 ||
      _useAlphaFromAlbedoSampler ||
      _opaqueTextureEnabled)
  {
    _transparentTechniqueEnabled = true;
  }
  else _opaqueTechniqueEnabled = true;

  _emissionTechniqueEnabled = _materialData->emission != glm::vec3(0) ||
                                          _materialData->reflectionFactor != 0;
}

void MeshCompositeColorTechnique::registerVariable(
                                                AbstractMeshVariable& variable,
                                                const std::string& name)
{
  CompositeMeshTechnique::registerVariable(variable, name);
  if(name == MeshExtraData::surfaceMaterialVariableName)
  {
    MeshExtraData::SurfaceMaterialVariableType& materialVariable =
                  static_cast<MeshExtraData::SurfaceMaterialVariableType&>(
                                                                      variable);
    _materialData.emplace(materialVariable.value());
    _updateTechniquesFlags();
  }
  if(name == MeshExtraData::alphaInAlbedoSamplerIsOpacityVariableName)
  {
    MeshBoolVariable& useAlphaVariable =
                                      static_cast<MeshBoolVariable&>(variable);
    _useAlphaFromAlbedoSampler = useAlphaVariable.value();
    _updateTechniquesFlags();
  }
}

void MeshCompositeColorTechnique::unregisterVariable(
                                              AbstractMeshVariable& variable,
                                              const std::string& name) noexcept
{
  CompositeMeshTechnique::unregisterVariable(variable, name);
  if(name == MeshExtraData::surfaceMaterialVariableName)
  {
    _materialData.reset();
    _updateTechniquesFlags();
  }
  if (name == MeshExtraData::alphaInAlbedoSamplerIsOpacityVariableName)
  {
    _useAlphaFromAlbedoSampler = false;
    _updateTechniquesFlags();
  }
}

void MeshCompositeColorTechnique::registerSampler(Sampler& sampler,
                                                  const std::string& name)
{
  CompositeMeshTechnique::registerSampler(sampler, name);

  if(name == MeshExtraData::opaqueSamplerName)
  {
    _opaqueTextureEnabled = true;
    _updateTechniquesFlags();
  }
}

void MeshCompositeColorTechnique::unregisterSampler(
                                              Sampler& sampler,
                                              const std::string& name) noexcept
{
  CompositeMeshTechnique::unregisterSampler(sampler, name);

  if(name == MeshExtraData::opaqueSamplerName)
  {
    _opaqueTextureEnabled = false;
    _updateTechniquesFlags();
  }
}

void MeshCompositeColorTechnique::addToDrawPlan(DrawPlanBuildInfo& buildInfo)
{
  if(_opaqueTechniqueEnabled) _opaqueTechnique.addToDrawPlan(buildInfo);
  if(_transparentTechniqueEnabled)
  {
    _transparentTechnique.addToDrawPlan(buildInfo);
  }
  if(_emissionTechniqueEnabled) _emissionTechnique.addToDrawPlan(buildInfo);
  if(buildInfo.selected) _selectionTechnique.addToDrawPlan(buildInfo);
}
