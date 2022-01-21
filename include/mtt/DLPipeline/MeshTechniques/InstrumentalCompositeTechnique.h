#pragma once

#include <mtt/DLPipeline/MeshTechniques/SelectionMeshTechnique.h>
#include <mtt/DLPipeline/MeshTechniques/InstrumentalTechnique.h>
#include <mtt/Render/Mesh/CompositeMeshTechnique.h>

namespace mtt
{
  namespace DLPipeline
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
