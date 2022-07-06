#pragma once

#include <mtt/clPipeline/Lighting/PointLightData.h>
#include <mtt/clPipeline/Lighting/LightAreaModificator.h>
#include <mtt/render/Pipeline/VolatileUniform.h>
#include <mtt/render/ImageView.h>

namespace mtt
{
  namespace clPipeline
  {
    class PointLight;

    class PointLightAreaModificator : public LightAreaModificator
    {
    private:
      friend class PointLight;
      explicit PointLightAreaModificator(PointLight& light);

    public:
      PointLightAreaModificator(const PointLightAreaModificator&) = delete;
      PointLightAreaModificator& operator = (
                                    const PointLightAreaModificator&) = delete;
      virtual ~PointLightAreaModificator() noexcept = default;

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
                                  const PointLightData& lightData);
      void _makeShadowCommand(DrawPlanBuildInfo& buildInfo,
                              const PointLightData& lightData,
                              ImageView& opaqueShadowmap,
                              ImageView& transparentShadowmap);

    private:
      PointLight& _light;

      VolatileUniform<PointLightData> _lightDataUniform;
    };

    inline void PointLightAreaModificator::reset() noexcept
    {
      resetConsumers();
    }
  }
}