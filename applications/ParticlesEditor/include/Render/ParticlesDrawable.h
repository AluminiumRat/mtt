#pragma once

#include <optional>

#include <glm/vec4.hpp>

#include <mtt/render/Drawable/Drawable.h>
#include <mtt/render/DrawPlan/DrawPlanBuildInfo.h>
#include <mtt/render/Pipeline/Buffer.h>
#include <mtt/render/Pipeline/GraphicsPipeline.h>
#include <mtt/render/Pipeline/VolatileUniform.h>

namespace mtt
{
  class AbstractRenderPass;
}

class ParticlesDrawable : public mtt::Drawable
{
public:
  ParticlesDrawable();
  ParticlesDrawable(const ParticlesDrawable&) = delete;
  ParticlesDrawable& operator = (const ParticlesDrawable&) = delete;
  virtual ~ParticlesDrawable() = default;

  /// w component of position should be 1.f
  /// x component of sizeRotation is diameter of particle, y component of
  /// sizeRotation is rotation of particle in radians
  /// rgb components of colorTransparency is premultipled color, a component
  /// is transparency
  void setData( size_t particlesNumber,
                glm::vec4* positionData,
                glm::vec4* sizeRotationData,
                glm::vec4* colorTransparencyData);

protected:
  virtual void buildDrawActions(mtt::DrawPlanBuildInfo& buildInfo) override;

private:
  class DrawTechnique
  {
  public:
    DrawTechnique(ParticlesDrawable& parent);

    void resetPipeline() noexcept;
    void buildDrawActions(mtt::DrawPlanBuildInfo& buildInfo);

  private:
    void _rebuildPipeline(mtt::AbstractRenderPass& renderPass);

  private:
    ParticlesDrawable& _parent;
    using MatricesUniform = mtt::VolatileUniform<mtt::DrawMatrices>;
    MatricesUniform _matricesUniform;
    std::optional<mtt::GraphicsPipeline> _pipeline;
  };

private:
  mtt::Buffer _positionBuffer;
  mtt::Buffer _sizeRotationBuffer;
  mtt::Buffer _colorTransparencyBuffer;
  size_t _particlesNumber;

  DrawTechnique _colorTechnique;
};