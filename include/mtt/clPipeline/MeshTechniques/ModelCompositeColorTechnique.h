#pragma once

#include <optional>
#include <memory>

#include <mtt/clPipeline/MeshTechniques/EmissionTechnique.h>
#include <mtt/clPipeline/MeshTechniques/OpaqueColorTechnique.h>
#include <mtt/clPipeline/MeshTechniques/SelectionMeshTechnique.h>
#include <mtt/clPipeline/MeshTechniques/TransparentProxyTechnique.h>
#include <mtt/render/Mesh/CompositeMeshTechnique.h>
#include <mtt/render/Mesh/MeshExtraData.h>

namespace mtt
{
  namespace clPipeline
  {
    class ModelCompositeColorTechnique : public CompositeMeshTechnique
    {
    public:
      explicit ModelCompositeColorTechnique(bool enableSelectionTechnique,
                                            VkPrimitiveTopology topology);
      ModelCompositeColorTechnique(
                                  const ModelCompositeColorTechnique&) = delete;
      ModelCompositeColorTechnique& operator = (
                                  const ModelCompositeColorTechnique&) = delete;
      virtual ~ModelCompositeColorTechnique() noexcept = default;

      virtual void registerVariable(AbstractMeshVariable& variable,
                                    const std::string& name) override;
      virtual void unregisterVariable(
                                    AbstractMeshVariable& variable,
                                    const std::string& name) noexcept override;

      virtual void registerSampler( Sampler& sampler,
                                    const std::string& name) override;
      virtual void unregisterSampler(
                                    Sampler& sampler,
                                    const std::string& name) noexcept override;

      virtual void addToDrawPlan(DrawPlanBuildInfo& buildInfo) override;

    private:
      void _updateTechniques();

    private:
      VkPrimitiveTopology _topology;
      bool _needUpdateTechniques;

      std::unique_ptr<OpaqueColorTechnique> _opaqueTechnique;
      std::unique_ptr<TransparentProxyTechnique> _transparentTechnique;
      std::unique_ptr<EmissionTechnique> _emissionTechnique;
      std::unique_ptr<SelectionMeshTechnique> _selectionTechnique;

      std::optional<SurfaceMaterialData> _materialData;
      bool _useAlphaFromAlbedoSampler;
      bool _opaqueTextureEnabled;
    };
  }
}