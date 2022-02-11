#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/clPipeline/MeshTechniques/ModelCompositeColorTechnique.h>

using namespace mtt;
using namespace mtt::clPipeline;

ModelCompositeColorTechnique::ModelCompositeColorTechnique(
                                                bool enableSelectionTechnique,
                                                VkPrimitiveTopology topology) :
  _topology(topology),
  _needUpdateTechniques(true),
  _useAlphaFromAlbedoSampler(false),
  _opaqueTextureEnabled(false)
{
  if(enableSelectionTechnique)
  {
    _selectionTechnique.reset(new SelectionMeshTechnique(topology));
    registerSubtechnique(*_selectionTechnique);
  }
}

void ModelCompositeColorTechnique::_updateTechniques()
{
  bool opaqueTechniqueEnabled = false;
  bool transparentTechniqueEnabled = false;
  bool emissionTechniqueEnabled = false;

  if(!_materialData.has_value())
  {
    opaqueTechniqueEnabled = true;
  }
  else
  {
    if( _materialData->opaque != 1 ||
        _useAlphaFromAlbedoSampler ||
        _opaqueTextureEnabled)
    {
      transparentTechniqueEnabled = true;
    }
    else
    {
      opaqueTechniqueEnabled = true;
      emissionTechniqueEnabled = _materialData->emission != glm::vec3(0) ||
                                          _materialData->reflectionFactor != 0;
    }
  }

  if (opaqueTechniqueEnabled && _opaqueTechnique == nullptr)
  {
    _opaqueTechnique.reset(new OpaqueColorTechnique(_topology));
    registerSubtechnique(*_opaqueTechnique);
  }
  if (!opaqueTechniqueEnabled && _opaqueTechnique != nullptr)
  {
    unregisterSubtechnique(*_opaqueTechnique);
    _opaqueTechnique.reset();
  }

  if (transparentTechniqueEnabled && _transparentTechnique == nullptr)
  {
    _transparentTechnique.reset(new TransparentProxyTechnique(_topology));
    registerSubtechnique(*_transparentTechnique);
  }
  if (!transparentTechniqueEnabled && _transparentTechnique != nullptr)
  {
    unregisterSubtechnique(*_transparentTechnique);
    _transparentTechnique.reset();
  }

  if (emissionTechniqueEnabled && _emissionTechnique == nullptr)
  {
    _emissionTechnique.reset(new EmissionTechnique(_topology));
    registerSubtechnique(*_emissionTechnique);
  }
  if (!emissionTechniqueEnabled && _emissionTechnique != nullptr)
  {
    unregisterSubtechnique(*_emissionTechnique);
    _emissionTechnique.reset();
  }
}

void ModelCompositeColorTechnique::registerVariable(
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
    _needUpdateTechniques = true;
  }
  if(name == MeshExtraData::alphaInAlbedoSamplerIsOpacityVariableName)
  {
    MeshBoolVariable& useAlphaVariable =
                                      static_cast<MeshBoolVariable&>(variable);
    _useAlphaFromAlbedoSampler = useAlphaVariable.value();
    _needUpdateTechniques = true;
  }
}

void ModelCompositeColorTechnique::unregisterVariable(
                                              AbstractMeshVariable& variable,
                                              const std::string& name) noexcept
{
  CompositeMeshTechnique::unregisterVariable(variable, name);
  if(name == MeshExtraData::surfaceMaterialVariableName)
  {
    _materialData.reset();
    _needUpdateTechniques = true;
  }
  if (name == MeshExtraData::alphaInAlbedoSamplerIsOpacityVariableName)
  {
    _useAlphaFromAlbedoSampler = false;
    _needUpdateTechniques = true;
  }
}

void ModelCompositeColorTechnique::registerSampler(Sampler& sampler,
                                                  const std::string& name)
{
  CompositeMeshTechnique::registerSampler(sampler, name);

  if(name == MeshExtraData::opaqueSamplerName)
  {
    _opaqueTextureEnabled = true;
    _needUpdateTechniques = true;
  }
}

void ModelCompositeColorTechnique::unregisterSampler(
                                              Sampler& sampler,
                                              const std::string& name) noexcept
{
  CompositeMeshTechnique::unregisterSampler(sampler, name);

  if(name == MeshExtraData::opaqueSamplerName)
  {
    _opaqueTextureEnabled = false;
    _needUpdateTechniques = true;
  }
}

void ModelCompositeColorTechnique::addToDrawPlan(DrawPlanBuildInfo& buildInfo)
{
  if(_needUpdateTechniques) _updateTechniques();

  if (_opaqueTechnique != nullptr) _opaqueTechnique->addToDrawPlan(buildInfo);

  if (_transparentTechnique != nullptr)
  {
    _transparentTechnique->addToDrawPlan(buildInfo);
  }

  if (_emissionTechnique != nullptr)
  {
    _emissionTechnique->addToDrawPlan(buildInfo);
  }

  if(_selectionTechnique != nullptr)
  {
    const bool* selectedPtr = static_cast<const bool*>(
          buildInfo.extraData.data(DrawPlanBuildExtraData::selectedFlagIndex));
    if (selectedPtr != nullptr && *selectedPtr == true)
    {
      _selectionTechnique->addToDrawPlan(buildInfo);
    }
  }
}
