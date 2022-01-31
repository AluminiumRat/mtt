#pragma once

#include <mtt/clPipeline/MeshTechniques/SelectionMeshTechnique.h>
#include <mtt/clPipeline/MeshTechniques/TransparentColorTechnique.h>
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

    private:
      TransparentColorTechnique _colorTechnique;
      SelectionMeshTechnique _selectionTechnique;
    };
  }
}
