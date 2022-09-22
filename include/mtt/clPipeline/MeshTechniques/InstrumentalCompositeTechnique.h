#pragma once

#include <mtt/clPipeline/MeshTechniques/SelectionMeshTechnique.h>
#include <mtt/clPipeline/MeshTechniques/UnlightedColorTechnique.h>
#include <mtt/render/Mesh/CompositeMeshTechnique.h>

namespace mtt
{
  namespace clPipeline
  {
    class InstrumentalCompositeTechnique : public CompositeMeshTechnique
    {
    public:
      InstrumentalCompositeTechnique( VkPrimitiveTopology topology,
                                      bool depthTestEnabled,
                                      bool depthWriteEnabled);
      InstrumentalCompositeTechnique(
                                const InstrumentalCompositeTechnique&) = delete;
      InstrumentalCompositeTechnique& operator = (
                                const InstrumentalCompositeTechnique&) = delete;
      virtual ~InstrumentalCompositeTechnique() noexcept = default;

      virtual void addToDrawPlan(DrawPlanBuildInfo& buildInfo) override;

      inline UnlightedColorTechnique& colorTechnique() noexcept;
      inline const UnlightedColorTechnique& colorTechnique() const noexcept;
      inline SelectionMeshTechnique& selectionTechnique() noexcept;
      inline const SelectionMeshTechnique& selectionTechnique() const noexcept;

    private:
      UnlightedColorTechnique _colorTechnique;
      SelectionMeshTechnique _selectionTechnique;
    };

    inline UnlightedColorTechnique&
                      InstrumentalCompositeTechnique::colorTechnique() noexcept
    {
      return _colorTechnique;
    }

    inline const UnlightedColorTechnique&
                InstrumentalCompositeTechnique::colorTechnique() const noexcept
    {
      return _colorTechnique;
    }

    inline SelectionMeshTechnique&
                  InstrumentalCompositeTechnique::selectionTechnique() noexcept
    {
      return _selectionTechnique;
    }

    inline const SelectionMeshTechnique&
            InstrumentalCompositeTechnique::selectionTechnique() const noexcept
    {
      return _selectionTechnique;
    }
  }
}
