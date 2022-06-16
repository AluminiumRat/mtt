#pragma once

#include <mtt/clPipeline/Lighting/SpotLightData.h>
#include <mtt/clPipeline/Lighting/LightAreaModificator.h>
#include <mtt/render/Pipeline/VolatileUniform.h>
#include <mtt/render/ImageView.h>

namespace mtt
{
  namespace clPipeline
  {
    class SpotLight;

    class SpotLightAreaModificator : public LightAreaModificator
    {
    private:
      friend class SpotLight;
      explicit SpotLightAreaModificator(SpotLight& light);

    public:
      SpotLightAreaModificator(const SpotLightAreaModificator&) = delete;
      SpotLightAreaModificator& operator = (
                                    const SpotLightAreaModificator&) = delete;
      virtual ~SpotLightAreaModificator() noexcept = default;

      virtual void adjustPipeline(AbstractPipeline& targetPipeline,
                                  ShaderModule& targetShader,
                                  ApplyModel applyModel,
                                  size_t modificatorIndex) override;

      inline void reset() noexcept;

      void updateBound() noexcept;

    protected:
      virtual void buildPrepareActions(DrawPlanBuildInfo& buildInfo) override;

    private:
      void _makeNonshadowCommand( DrawPlanBuildInfo& buildInfo,
                                  const SpotLightData& lightData);
      void _makeShadowCommand(DrawPlanBuildInfo& buildInfo,
                              const SpotLightData& lightData,
                              ImageView& shadowMapView);

    private:
      SpotLight& _light;

      VolatileUniform<SpotLightData> _lightDataUniform;
    };

    inline void SpotLightAreaModificator::reset() noexcept
    {
      resetConsumers();
    }
  }
}