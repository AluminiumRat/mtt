#pragma once

#include <mtt/dlPipeline/MeshTechniques/SelectionMeshTechnique.h>
#include <mtt/dlPipeline/MeshTechniques/InstrumentalTechnique.h>
#include <mtt/render/Mesh/CompositeMeshTechnique.h>

namespace mtt
{
  namespace dlPipeline
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

    private:
      InstrumentalTechnique _colorTechnique;
      SelectionMeshTechnique _selectionTechnique;
    };
  }
}
