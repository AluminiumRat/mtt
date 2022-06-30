#pragma once

#include <vector>

#include <mtt/clPipeline/Lighting/CascadeShadowMapProvider.h>
#include <mtt/clPipeline/Lighting/DirectLightData.h>
#include <mtt/clPipeline/Lighting/LightAreaModificator.h>
#include <mtt/render/Pipeline/VolatileUniform.h>

namespace mtt
{
  namespace clPipeline
  {
    class DirectLight;

    class DirectLightAreaModificator : public LightAreaModificator
    {
    private:
      friend class DirectLight;
      explicit DirectLightAreaModificator(DirectLight& light);

    public:
      DirectLightAreaModificator(const DirectLightAreaModificator&) = delete;
      DirectLightAreaModificator& operator = (
                                    const DirectLightAreaModificator&) = delete;
      virtual ~DirectLightAreaModificator() noexcept = default;

      virtual void adjustPipeline(AbstractPipeline& targetPipeline,
                                  ShaderModule& targetShader,
                                  ApplyModel applyModel,
                                  size_t modificatorIndex) override;

      inline void reset() noexcept;

    protected:
      virtual void buildPrepareActions(DrawPlanBuildInfo& buildInfo) override;

    private:
      void _makeNonshadowCommand( DrawPlanBuildInfo& buildInfo,
                                  const DirectLightData& lightData);
      void _makeShadowCommand(
                      DrawPlanBuildInfo& buildInfo,
                      const DirectLightData& lightData,
                      const CascadeShadowMapProvider::CascadeInfo& cascadeInfo);

    private:
      DirectLight& _light;

      VolatileUniform<DirectLightData> _lightDataUniform;

      using CoordsCorrectionData = std::vector<glm::vec4>;
      VolatileUniform<CoordsCorrectionData> _coordsCorrectionUniform;
    };

    inline void DirectLightAreaModificator::reset() noexcept
    {
      resetConsumers();
    }
  }
}