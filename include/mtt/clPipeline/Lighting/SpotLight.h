#pragma once

#include <optional>
#include <memory>

#include <mtt/clPipeline/Lighting/SpotLightApplicator.h>
#include <mtt/clPipeline/Lighting/SpotLightAreaModificator.h>
#include <mtt/clPipeline/Lighting/SpotLightData.h>
#include <mtt/render/Pipeline/Sampler.h>
#include <mtt/render/Pipeline/Texture2D.h>
#include <mtt/render/SceneGraph/CameraNode.h>
#include <mtt/render/SceneGraph/CompositeObjectNode.h>
#include <mtt/utilities/Sphere.h>

namespace mtt
{
  struct DrawPlanBuildInfo;

  namespace clPipeline
  {
    class ShadowMapProvider;

    class SpotLight : public CompositeObjectNode
    {
    public:
      SpotLight(bool forwardLightingEnabled,
                bool defferedLightingEnabled,
                LogicalDevice& device);
      SpotLight(const SpotLight&) = delete;
      SpotLight& operator = (const SpotLight&) = delete;
      virtual ~SpotLight() = default;

      inline const glm::vec3& illuminance() const noexcept;
      inline void setIlluminance(const glm::vec3& newValue) noexcept;

      inline float distance() const noexcept;
      inline void setDistance(float newValue) noexcept;

      inline float angle() const noexcept;
      inline void setAngle(float newValue) noexcept;

      inline Texture2D* filterTexture() const noexcept;
      /// You can set nullptr to remove filter texture
      void setFilterTexture(std::shared_ptr<Texture2D> newTexture);

      inline const CameraNode& shadowmapCamera() const noexcept;

      inline ShadowMapProvider* shadowMapProvider() const noexcept;
      void setShadowMapProvider(ShadowMapProvider* newProvider) noexcept;

      inline float blurAngle() const noexcept;
      inline void setBlurAngle(float newValue) noexcept;

      inline Sphere getBoundSphere() const noexcept;

      virtual size_t culledDrawablesNumber() const noexcept override;
      virtual DrawableNode& culledDrawable(size_t index) noexcept override;

      virtual size_t areaModificatorsNumber() const noexcept override;
      virtual AreaModificator& areaModificator(size_t index) noexcept override;

    private:
      friend class SpotLightApplicator;
      friend class SpotLightAreaModificator;
      SpotLightData buildDrawData(
                            const DrawPlanBuildInfo& buildInfo) const noexcept;
      inline Sampler* filterSampler() noexcept;
      inline Sampler* shadowmapSampler() noexcept;

    private:
      void _updateShadowmapCamera() noexcept;
      void _updateBound() noexcept;
      void _resetPipelines() noexcept;

    private:
      LogicalDevice& _device;

      CameraNode _shadowmapCamera;
      ShadowMapProvider* _shadowMapProvider;
      std::optional<Sampler> _shadowmapSampler;

      glm::vec3 _illuminance;
      float _distance;
      float _angle;
      float _blurAngle;

      std::optional<Sampler> _filterSampler;

      std::unique_ptr<SpotLightApplicator> _defferedLightApplicator;
      std::unique_ptr<SpotLightAreaModificator> _forwardLightApplicator;
    };

    inline const glm::vec3& SpotLight::illuminance() const noexcept
    {
      return _illuminance;
    }

    inline void SpotLight::setIlluminance(const glm::vec3& newValue) noexcept
    {
      glm::vec3 newIlluminance = glm::max(glm::vec3(0.f), newValue);
      _illuminance = newIlluminance;
    }

    inline float SpotLight::distance() const noexcept
    {
      return _distance;
    }

    inline void SpotLight::setDistance(float newValue) noexcept
    {
      newValue = glm::max(0.f, newValue);
      if (_distance == newValue) return;
      _distance = newValue;
      _updateShadowmapCamera();
      _updateBound();
    }

    inline float SpotLight::angle() const noexcept
    {
      return _angle;
    }

    inline void SpotLight::setAngle(float newValue) noexcept
    {
      newValue = glm::clamp(newValue, 0.f, glm::pi<float>() / 2.f);
      if (_angle == newValue) return;
      _angle = newValue;
      _updateShadowmapCamera();
      _updateBound();
    }

    inline Texture2D* SpotLight::filterTexture() const noexcept
    {
      if(!_filterSampler.has_value()) return nullptr;
      return static_cast<Texture2D*>(_filterSampler->attachedTexture(0));
    }

    inline const CameraNode& SpotLight::shadowmapCamera() const noexcept
    {
      return _shadowmapCamera;
    }

    inline ShadowMapProvider* SpotLight::shadowMapProvider() const noexcept
    {
      return _shadowMapProvider;
    }

    inline float SpotLight::blurAngle() const noexcept
    {
      return _blurAngle;
    }

    inline void SpotLight::setBlurAngle(float newValue) noexcept
    {
      newValue = glm::clamp(newValue, 0.f, glm::pi<float>() / 10.f);
      _blurAngle = newValue;
      _updateShadowmapCamera();
    }

    inline Sphere SpotLight::getBoundSphere() const noexcept
    {
      float halfdistance = distance() / 2;
      float baseRadius = distance() * tan(angle() / 2.f);
      float boundRadius = sqrt(halfdistance * halfdistance +
                                                      baseRadius * baseRadius);
      return Sphere(glm::vec3(0.f, 0.f, -halfdistance), boundRadius);
    }

    inline Sampler* SpotLight::filterSampler() noexcept
    {
      if(_filterSampler.has_value()) return &_filterSampler.value();
      return nullptr;
    }

    inline Sampler* SpotLight::shadowmapSampler() noexcept
    {
      if(_shadowmapSampler.has_value()) return &_shadowmapSampler.value();
      return nullptr;
    }

  }
}