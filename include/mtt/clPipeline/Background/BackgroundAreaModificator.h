#pragma once

#include <mtt/clPipeline/Background/BackgroundProperties.h>
#include <mtt/render/SceneGraph/AreaModificator.h>
#include <mtt/render/Pipeline/Sampler.h>
#include <mtt/render/Pipeline/VolatileUniform.h>

namespace mtt
{
  namespace clPipeline
  {
    class BackgroundAreaModificator : public AreaModificator
    {
    public:
      static const size_t typeIndex;

    public:
      BackgroundAreaModificator(const BackgroundProperties& properties,
                                Sampler& luminanceSampler);
      BackgroundAreaModificator(const BackgroundAreaModificator&) = delete;
      BackgroundAreaModificator& operator = (
                                    const BackgroundAreaModificator&) = delete;
      virtual ~BackgroundAreaModificator() = default;

      void adjustPipeline(AbstractPipeline& targetPipeline,
                          ShaderModule& targetShader,
                          size_t modificatorIndex);

      inline void reset() noexcept;

    protected:
      virtual void buildPrepareActions(DrawPlanBuildInfo& buildInfo) override;

    private:
      const BackgroundProperties& _properties;
      Sampler& _luminanceSampler;
      VolatileUniform<BackgroundDrawData> _drawDataUniform;
    };

    inline void BackgroundAreaModificator::reset() noexcept
    {
      resetConsumers();
    }
  }
}