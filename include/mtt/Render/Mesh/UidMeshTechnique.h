#pragma once

#include <optional>

#include <mtt/Render/Mesh/BaseGeometryTechnique.h>

namespace mtt
{
  /// Draws an objectUid from buildInfo to red(low 32 bits) and
  /// green(high 32 bits) components. FramePlan::UID_STAGE is used.
  /// Only the position buffer is required.
  class UidMeshTechnique : public BaseGeometryTechnique
  {
  public:
    UidMeshTechnique( VkPrimitiveTopology topology,
                      bool depthTestEnabled,
                      bool depthWriteEnabled);
    UidMeshTechnique(const UidMeshTechnique&) = delete;
    UidMeshTechnique& operator = (const UidMeshTechnique&) = delete;
    virtual ~UidMeshTechnique()  noexcept = default;

  protected:
    virtual void invalidatePipeline() noexcept override;
    virtual void adjustPipeline(GraphicsPipeline& pipeline,
                                AbstractRenderPass& renderPass) override;
    virtual void createRenderAction(
                                  DrawPlanBuildInfo& buildInfo,
                                  GraphicsPipeline& pipeline,
                                  MatricesUniform& matricesUniform,
                                  const BoneMatrices* boneMatricesPtr) override;
  private:
    bool _depthTestEnabled;
    bool _depthWriteEnabled;
    using UidUniform = VolatileUniform<glm::uvec2>;
    std::optional<UidUniform> _uidUniform;
  };
}