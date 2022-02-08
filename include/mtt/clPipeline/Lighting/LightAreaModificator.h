#pragma once

#include <mtt/render/SceneGraph/AreaModificator.h>

namespace mtt
{
  class AbstractPipeline;
  class ShaderModule;

  namespace clPipeline
  {
    class LightAreaModificator : public AreaModificator
    {
    public:
      static const size_t typeIndex;

    public:
      LightAreaModificator();
      LightAreaModificator(const LightAreaModificator&) = delete;
      LightAreaModificator& operator = (const LightAreaModificator&) = delete;
      virtual ~LightAreaModificator() noexcept = default;

      virtual void adjustPipeline(AbstractPipeline& targetPipeline,
                                  ShaderModule& targetShader,
                                  size_t modificatorIndex) = 0;
    };
  }
}