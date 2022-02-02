#pragma once

#include <mtt/render/Mesh/ModificatorProxyTechnique.h>

namespace mtt
{
  namespace clPipeline
  {
    class TransparentProxyTechnique : public ModificatorProxyTechnique
    {
    public:
      explicit TransparentProxyTechnique(VkPrimitiveTopology topology);
      TransparentProxyTechnique(const TransparentProxyTechnique&) = delete;
      TransparentProxyTechnique& operator = (
                                    const TransparentProxyTechnique&) = delete;
      virtual ~TransparentProxyTechnique() noexcept = default;

      inline VkPrimitiveTopology topology() const noexcept;

    protected:
      virtual std::unique_ptr<AbstractMeshTechnique>
                              createTechnique(AreaModificatorSet& set) override;

    private:
      VkPrimitiveTopology _topology;
    };

    inline VkPrimitiveTopology
                            TransparentProxyTechnique::topology() const noexcept
    {
      return _topology;
    }
  }
}