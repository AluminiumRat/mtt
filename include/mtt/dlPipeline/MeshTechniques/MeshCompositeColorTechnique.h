#pragma once

#include <optional>

#include <mtt/dlPipeline/MeshTechniques/EmissionTechnique.h>
#include <mtt/dlPipeline/MeshTechniques/OpaqueColorTechnique.h>
#include <mtt/dlPipeline/MeshTechniques/SelectionMeshTechnique.h>
#include <mtt/dlPipeline/MeshTechniques/TransparentColorTechnique.h>
#include <mtt/render/Mesh/CompositeMeshTechnique.h>
#include <mtt/render/Mesh/MeshExtraData.h>

namespace mtt
{
  namespace dlPipeline
  {
    class MeshCompositeColorTechnique : public CompositeMeshTechnique
    {
    public:
      explicit MeshCompositeColorTechnique(VkPrimitiveTopology topology);
      MeshCompositeColorTechnique(const MeshCompositeColorTechnique&) = delete;
      MeshCompositeColorTechnique& operator = (
                                  const MeshCompositeColorTechnique&) = delete;
      virtual ~MeshCompositeColorTechnique() noexcept = default;

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
      void _updateTechniquesFlags() noexcept;

    private:
      OpaqueColorTechnique _opaqueTechnique;
      bool _opaqueTechniqueEnabled;

      TransparentColorTechnique _transparentTechnique;
      bool _transparentTechniqueEnabled;

      EmissionTechnique _emissionTechnique;
      bool _emissionTechniqueEnabled;

      SelectionMeshTechnique _selectionTechnique;

      std::optional<SurfaceMaterialData> _materialData;
      bool _useAlphaFromAlbedoSampler;
      bool _opaqueTextureEnabled;
    };
  }
}