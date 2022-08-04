#pragma once

#include <mtt/particles/Drawable/ParticlesDrawCommonData.h>
#include <mtt/render/Drawable/PipelineDrawable.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/Pipeline/VolatileUniform.h>

namespace mtt
{
  class AbstractRenderPass;

  class ParticlesAbstractTechnique : public PipelineDrawable
  {
  public:
    ParticlesAbstractTechnique( ParticlesDrawCommonData& commonData,
                                FrameType frameType,
                                StageIndex stageIndex,
                                uint8_t thinningFactor);
    ParticlesAbstractTechnique(const ParticlesAbstractTechnique&) = delete;
    ParticlesAbstractTechnique& operator = (
                                    const ParticlesAbstractTechnique&) = delete;
    virtual ~ParticlesAbstractTechnique() noexcept = default;

  protected:
    inline ParticlesDrawCommonData& commonData() const noexcept;

    virtual void adjustPipeline(GraphicsPipeline& pipeline) override;
    virtual void buildDrawActions(DrawPlanBuildInfo& buildInfo) override;

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

    struct IndicesData
    {
      Ref<PlainBuffer> buffer;
      size_t pointsNumber;
    };

    IndicesData _makeIndices( const DrawPlanBuildInfo& buildInfo,
                              LogicalDevice& device) const;

  private:
    ParticlesDrawCommonData& _commonData;
    uint8_t _thinningFactor;
    VolatileUniform<DrawMatrices> _matricesUniform;
    VolatileUniform<MppxDistanceFunction> _mppxFunctionUniform;
    VolatileUniform<glm::vec2> _fallofUniform;
  };

  inline ParticlesDrawCommonData&
                        ParticlesAbstractTechnique::commonData() const noexcept
  {
    return _commonData;
  }
}
