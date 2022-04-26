#pragma once

#include <mtt/clPipeline/Lighting/AmbientLightData.h>
#include <mtt/clPipeline/Lighting/LightAreaModificator.h>
#include <mtt/render/Pipeline/Sampler.h>
#include <mtt/render/Pipeline/VolatileUniform.h>

namespace mtt
{
  namespace clPipeline
  {
    class AmbientLightAreaModificator : public LightAreaModificator
    {
    public:
      AmbientLightAreaModificator(const AmbientLightData& lightData,
                                  Sampler& ambientMapSampler,
                                  Sampler& diffuseLuminanceSampler);
      AmbientLightAreaModificator(const AmbientLightAreaModificator&) = delete;
      AmbientLightAreaModificator& operator = (
                                  const AmbientLightAreaModificator&) = delete;
      virtual ~AmbientLightAreaModificator() = default;

      virtual void adjustPipeline(AbstractPipeline& targetPipeline,
                                  ShaderModule& targetShader,
                                  ApplyModel applyModel,
                                  size_t modificatorIndex) override;

      inline void reset() noexcept;

      void updateBound() noexcept;

    protected:
      virtual void buildPrepareActions(DrawPlanBuildInfo& buildInfo) override;

    private:
      const AmbientLightData& _lightData;
      VolatileUniform<AmbientLightDrawData> _drawDataUniform;
      Sampler& _ambientMapSampler;
      Sampler& _diffuseLuminanceSampler;
    };

    inline void AmbientLightAreaModificator::reset() noexcept
    {
      resetConsumers();
    }
  }
}