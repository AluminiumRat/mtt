#pragma once

#include <mtt/particles/Drawable/ParticlesDrawCommonData.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/Mesh/AbstractMeshTechnique.h>
#include <mtt/render/Pipeline/GraphicsPipeline.h>
#include <mtt/render/Pipeline/VolatileUniform.h>

namespace mtt
{
  class AbstractRenderPass;

  class ParticlesAbstractTechnique : public AbstractMeshTechnique
  {
  public:
    ParticlesAbstractTechnique( ParticlesDrawCommonData& commonData,
                                StageIndex stage,
                                uint8_t thinningFactor);
    ParticlesAbstractTechnique(const ParticlesAbstractTechnique&) = delete;
    ParticlesAbstractTechnique& operator = (
                                    const ParticlesAbstractTechnique&) = delete;
    virtual ~ParticlesAbstractTechnique() noexcept = default;

    inline StageIndex stage() const noexcept;

    void clearPipeline() noexcept;
    virtual void addToDrawPlan(DrawPlanBuildInfo& buildInfo) override;

  protected:
    inline ParticlesDrawCommonData& commonData() const noexcept;

    virtual void adjustPipeline(GraphicsPipeline& pipeline) = 0;
    virtual void buildDrawAction(
                    DrawBin& renderBin,
                    DrawPlanBuildInfo& buildInfo,
                    GraphicsPipeline& pipeline,
                    uint32_t pointsNumber,
                    PlainBuffer& indices,
                    VolatileUniform<DrawMatrices>& matricesUniform,
                    VolatileUniform<MppxDistanceFunction>& mppxFunctionUniform,
                    VolatileUniform<glm::vec2>& falloffUniform,
                    glm::vec2 falloffValue) = 0;

  private:
    std::string _makeTextureExtentDefine() const;
    void _rebuildPipeline(AbstractRenderPass& renderPass);

    struct IndicesData
    {
      Ref<PlainBuffer> buffer;
      size_t pointsNumber;
    };

    IndicesData _makeIndices( const DrawPlanBuildInfo& buildInfo,
                              LogicalDevice& device) const;

  private:
    ParticlesDrawCommonData& _commonData;
    StageIndex _stage;
    uint8_t _thinningFactor;
    VolatileUniform<DrawMatrices> _matricesUniform;
    VolatileUniform<MppxDistanceFunction> _mppxFunctionUniform;
    VolatileUniform<glm::vec2> _fallofUniform;
    std::optional<GraphicsPipeline> _pipeline;
  };

  inline ParticlesDrawCommonData&
                        ParticlesAbstractTechnique::commonData() const noexcept
  {
    return _commonData;
  }

  inline StageIndex ParticlesAbstractTechnique::stage() const noexcept
  {
    return _stage;
  }
}
