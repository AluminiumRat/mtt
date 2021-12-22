#pragma once

#include <vulkan/vulkan.h>

#include <mtt/Render/Mesh/MeshTechniquesFactory.h>

namespace mtt
{
  class ModelTechniquesFactory : public MeshTechniquesFactory
  {
  public:
    ModelTechniquesFactory( bool createColorTechnique,
                            bool createShadowmapTechnique,
                            bool createDepthTechnique,
                            bool createUidTechnique,
                            VkPrimitiveTopology topology);
    ModelTechniquesFactory(const ModelTechniquesFactory&) = delete;
    ModelTechniquesFactory& operator = (const ModelTechniquesFactory&) = delete;
    virtual ~ModelTechniquesFactory() noexcept = default;

    virtual void setupTechniques(Mesh& mesh) override;

  private:
    bool _createColorTechnique;
    bool _createShadowmapTechnique;
    bool _createDepthTechnique;
    bool _createUidTechnique;
    VkPrimitiveTopology _topology;
  };
}