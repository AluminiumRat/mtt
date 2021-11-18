#pragma once

#include <mtt/DrawImpl/MeshTechniques/SelectionMeshTechnique.h>
#include <mtt/DrawImpl/MeshTechniques/InstrumentalTechnique.h>
#include <mtt/Render/Mesh/CompositeMeshTechnique.h>

namespace mtt
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
