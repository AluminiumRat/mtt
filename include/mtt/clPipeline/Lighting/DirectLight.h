#pragma once

#include <mtt/clPipeline/Lighting/AbstractLight.h>
#include <mtt/clPipeline/Lighting/DirectLightApplicator.h>
#include <mtt/clPipeline/Lighting/DirectLightData.h>
#include <mtt/render/Pipeline/Sampler.h>
#include <mtt/render/Pipeline/Texture2D.h>
#include <mtt/utilities/Abort.h>
#include <mtt/utilities/Sphere.h>

namespace mtt
{
  namespace clPipeline
  {
    class ShadowMapProvider;

    class DirectLight : public AbstractLight
    {
    public:
      explicit DirectLight(LogicalDevice& device);
      DirectLight(const DirectLight&) = delete;
      DirectLight& operator = (const DirectLight&) = delete;
      virtual ~DirectLight() = default;

      virtual DrawableNode* defferedLightApplicator() noexcept override;
      virtual AreaModificator* forwardLightModificator() noexcept override;

      inline const glm::vec3& illuminance() const noexcept;
      inline void setIlluminance(const glm::vec3& newValue) noexcept;

      inline float distance() const noexcept;
      inline void setDistance(float newValue) noexcept;

      inline float radius() const noexcept;
      inline void setRadius(float newValue) noexcept;

      inline ShadowMapProvider* shadowMapProvider() const noexcept;
      inline void setShadowMapProvider(ShadowMapProvider* newProvider) noexcept;

      inline size_t cascadeSize() const noexcept;
      /// newValue should not be 0
      inline void setCascadeSize(size_t newValue) noexcept;

      inline float blurSize() const noexcept;
      inline void setBlurSize(float newValue) noexcept;

      inline Sphere getBoundSphere() const noexcept;

    private:
      void _updateBound() noexcept;
      void _resetPipelines() noexcept;

    private:
      ShadowMapProvider* _shadowMapProvider;
      glm::vec3 _illuminance;
      float _distance;
      float _radius;
      size_t _cascadeSize;
      float _blurSize;

      DirectLightApplicator _defferedLightApplicator;
    };

    inline const glm::vec3& DirectLight::illuminance() const noexcept
    {
      return _illuminance;
    }

    inline void DirectLight::setIlluminance(const glm::vec3& newValue) noexcept
    {
      _illuminance = newValue;
    }

    inline float DirectLight::distance() const noexcept
    {
      return _distance;
    }

    inline void DirectLight::setDistance(float newValue) noexcept
    {
      if(_distance == newValue) return;
      _distance = newValue;
      _updateBound();
    }

    inline float DirectLight::radius() const noexcept
    {
      return _radius;
    }

    inline void DirectLight::setRadius(float newValue) noexcept
    {
      if(_radius == newValue) return;
      _radius = newValue;
      _updateBound();
    }

    inline ShadowMapProvider* DirectLight::shadowMapProvider() const noexcept
    {
      return _shadowMapProvider;
    }

    inline void DirectLight::setShadowMapProvider(
                                        ShadowMapProvider* newProvider) noexcept
    {
      if(_shadowMapProvider == newProvider) return;
      _shadowMapProvider = newProvider;
      _resetPipelines();
    }

    inline size_t DirectLight::cascadeSize() const noexcept
    {
      return _cascadeSize;
    }

    inline void DirectLight::setCascadeSize(size_t newValue) noexcept
    {
      if (newValue == 0) Abort("DirectLight::setCascadeSize: new value is 0");
      if(_cascadeSize == newValue) return;
      _cascadeSize = newValue;
      _resetPipelines();
    }

    inline float DirectLight::blurSize() const noexcept
    {
      return _blurSize;
    }

    inline void DirectLight::setBlurSize(float newValue) noexcept
    {
      _blurSize = newValue;
    }

    inline Sphere DirectLight::getBoundSphere() const noexcept
    {
      float halfdistance = distance() / 2;
      float boundRadius = sqrt(halfdistance * halfdistance +
                                                          radius() * radius());
      return Sphere(glm::vec3(0.f, 0.f, -halfdistance), boundRadius);
    }
  }
}