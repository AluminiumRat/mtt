#pragma once

#include <mtt/clPipeline/Lighting/AmbientLightData.h>
#include <mtt/render/SceneGraph/AreaModificator.h>
#include <mtt/render/Pipeline/Sampler.h>
#include <mtt/render/Pipeline/VolatileUniform.h>

namespace mtt
{
  namespace clPipeline
  {
    class AmbientLightAreaModificator : public AreaModificator
    {
    public:
      static const size_t typeIndex;

    public:
      AmbientLightAreaModificator(const AmbientLightData& lightData,
                                  Sampler& ambientMapSampler,
                                  Sampler& diffuseLuminanceSampler);
      AmbientLightAreaModificator(const AmbientLightAreaModificator&) = delete;
      AmbientLightAreaModificator& operator = (
                                  const AmbientLightAreaModificator&) = delete;
      virtual ~AmbientLightAreaModificator() = default;

      void adjustPipeline(AbstractPipeline& targetPipeline,
                          ShaderModule& targetShader,
                          size_t modificatorIndex);

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