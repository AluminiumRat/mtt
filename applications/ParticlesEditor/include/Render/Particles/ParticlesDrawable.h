#pragma once

#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <mtt/render/Drawable/Drawable.h>

#include <Render/Particles/ParticlesDrawCommonData.h>
#include <Render/Particles/ParticlesProxyTechnique.h>
#include <Render/Particles/ParticlesShadowmapTechnique.h>

namespace mtt
{
  class AbstractRenderPass;
}

class ParticlesDrawable : public mtt::Drawable
{
public:
  using TextureData = ParticlesDrawCommonData::TextureData;

  enum LightingType
  {
    PER_PARTICLE_LIGHTING = ParticlesDrawCommonData::PER_PARTICLE_LIGHTING,
    PER_PIXEL_LIGHTING = ParticlesDrawCommonData::PER_PIXEL_LIGHTING
  };

public:
  ParticlesDrawable();
  ParticlesDrawable(const ParticlesDrawable&) = delete;
  ParticlesDrawable& operator = (const ParticlesDrawable&) = delete;
  virtual ~ParticlesDrawable() = default;

  /// All data vectors must have the same size
  /// x component of sizeRotation is diameter of particle,
  /// y component of sizeRotation is rotation of particle in radians
  /// albedoData contains premultipled rgba color value with opacity
  /// emissionData uses only rgb components
  /// tagData is used to smooth degradation and must be random number.
  /// If particlesNumber != 0 then all pointers must not be nullptr
  void setData( const std::vector<glm::vec3>& positionData,
                const std::vector<glm::vec2>& sizeRotationData,
                const std::vector<glm::vec4>& albedoData,
                const std::vector<glm::vec4>& emissionData,
                const std::vector<uint32_t>& textureIndexData,
                const std::vector<uint32_t>& tileIndexData,
                const std::vector<uint8_t>& tagData);

  void setParticleTextures(const std::vector<TextureData>& textures);

  inline float falloffBaseMppx() const noexcept;
  void setFalloffBaseMppx(float newValue) noexcept;
  inline float falloffSmoothing() const noexcept;
  void setFalloffSmoothing(float newValue) noexcept;

  inline LightingType lightingType() const noexcept;
  void setLightingType(LightingType newValue) noexcept;

  virtual void registerAreaModificators(mtt::AreaModificatorSet& set) override;
  virtual void unregisterAreaModificators(
                                mtt::AreaModificatorSet& set) noexcept override;

protected:
  virtual void buildDrawActions(mtt::DrawPlanBuildInfo& buildInfo) override;

private:
  ParticlesDrawCommonData _commonData;
  ParticlesProxyTechnique _colorTechnique;
  ParticlesShadowmapTechnique _shadowmapTechnique;
  float _randomFactorTable[256];
};

inline float ParticlesDrawable::falloffBaseMppx() const noexcept
{
  return _commonData.falloffBaseMppx;
}

inline float ParticlesDrawable::falloffSmoothing() const noexcept
{
  return _commonData.falloffSmoothing;
}

inline ParticlesDrawable::LightingType
                                ParticlesDrawable::lightingType() const noexcept
{
  return static_cast<LightingType>(_commonData.lightingType);
}
