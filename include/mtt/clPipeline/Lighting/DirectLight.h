#pragma once

#include <memory>
#include <optional>

#include <mtt/clPipeline/Lighting/CascadeShadowmapProvider.h>
#include <mtt/clPipeline/Lighting/DirectLightApplicator.h>
#include <mtt/clPipeline/Lighting/DirectLightAreaModificator.h>
#include <mtt/clPipeline/Lighting/DirectLightData.h>
#include <mtt/render/Pipeline/Sampler.h>
#include <mtt/render/SceneGraph/CompositeObjectNode.h>
#include <mtt/utilities/Abort.h>
#include <mtt/utilities/Sphere.h>

namespace mtt
{
  class AbstractField;
  struct DrawPlanBuildInfo;

  namespace clPipeline
  {
    class DirectLight : public CompositeObjectNode
    {
    public:
      explicit DirectLight( bool forwardLightingEnabled,
                            bool defferedLightingEnabled,
                            LogicalDevice& device);
      DirectLight(const DirectLight&) = delete;
      DirectLight& operator = (const DirectLight&) = delete;
      virtual ~DirectLight() = default;

      inline const glm::vec3& illuminance() const noexcept;
      inline void setIlluminance(const glm::vec3& newValue) noexcept;

      inline float distance() const noexcept;
      inline void setDistance(float newValue) noexcept;

      inline float radius() const noexcept;
      inline void setRadius(float newValue);

      inline const CameraNode& shadowmapCamera() const noexcept;

      inline AbstractField* shadowmapField() const noexcept;
      inline void setShadowmapField(AbstractField* newField);

      inline unsigned int shadowmapExtent() const noexcept;
      inline void setShadowmapExtent(unsigned int newValue);

      inline size_t cascadeSize() const noexcept;
      inline void setCascadeSize(size_t newValue);

      inline float blurSize() const noexcept;
      inline void setBlurSize(float newValue) noexcept;

      inline Sphere getBoundSphere() const noexcept;

      virtual size_t culledDrawablesNumber() const noexcept override;
      virtual DrawableNode& culledDrawable(size_t index) noexcept override;

      virtual size_t areaModificatorsNumber() const noexcept override;
      virtual AreaModificator& areaModificator(size_t index) noexcept override;

    private:
      friend class DirectLightApplicator;
      friend class DirectLightAreaModificator;
      DirectLightDrawData buildDrawData(
                            const DrawPlanBuildInfo& buildInfo) const noexcept;
      inline CascadeShadowMapProvider* shadowmapProvider() noexcept;
      inline Sampler* shadowmapSampler() noexcept;

      /// Blur radius in UV coordinates, i.e. blur radius relative to light
      /// diameter
      inline float blurRelativeRadius() const noexcept;

    private:
      void _resetShadowmapProvider() noexcept;
      void _updateShadowmapProvider();
      void _updateShadowmapCamera() noexcept;
      void _updateBound() noexcept;
      void _resetPipelines() noexcept;

    private:
      LogicalDevice& _device;

      CameraNode _shadowmapCamera;
      std::unique_ptr<CascadeShadowMapProvider> _shadowmapProvider;
      unsigned int _shadowmapExtent;
      AbstractField* _shadowmapField;
      std::unique_ptr<Sampler> _shadowmapSampler;

      glm::vec3 _illuminance;
      float _distance;
      float _radius;
      size_t _cascadeSize;
      float _blurSize;

      std::unique_ptr<DirectLightApplicator> _defferedLightApplicator;
      std::unique_ptr<DirectLightAreaModificator> _forwardLightApplicator;
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
      _updateShadowmapCamera();
      _updateBound();
    }

    inline float DirectLight::radius() const noexcept
    {
      return _radius;
    }

    inline void DirectLight::setRadius(float newValue)
    {
      newValue = glm::max(0.f, newValue);
      if(_radius == newValue) return;
      
      _radius = newValue;
      _updateBound();
      _updateShadowmapCamera();
      _updateShadowmapProvider();
    }

    inline const CameraNode& DirectLight::shadowmapCamera() const noexcept
    {
      return _shadowmapCamera;
    }

    inline CascadeShadowMapProvider* DirectLight::shadowmapProvider() noexcept
    {
      return _shadowmapProvider.get();
    }

    inline unsigned int DirectLight::shadowmapExtent() const noexcept
    {
      return _shadowmapExtent;
    }

    inline void DirectLight::setShadowmapExtent(unsigned int newValue)
    {
      if (_shadowmapExtent == newValue) return;
      _shadowmapExtent = newValue;
      _updateShadowmapProvider();
      if (_shadowmapProvider != nullptr)
      {
        try
        {
          _shadowmapProvider->setFrameExtent(glm::uvec2(_shadowmapExtent));
        }
        catch (...)
        {
          _resetShadowmapProvider();
          throw;
        }
      }
    }

    inline AbstractField* DirectLight::shadowmapField() const noexcept
    {
      return _shadowmapField;
    }

    inline void DirectLight::setShadowmapField(AbstractField* newField)
    {
      if(_shadowmapField == newField) return;
      _shadowmapField = newField;
      _updateShadowmapProvider();
      if(_shadowmapProvider != nullptr)
      {
        _shadowmapProvider->setTargetField(_shadowmapField);
      }
    }

    inline size_t DirectLight::cascadeSize() const noexcept
    {
      return _cascadeSize;
    }

    inline void DirectLight::setCascadeSize(size_t newValue)
    {
      if(_cascadeSize == newValue) return;
      _cascadeSize = newValue;
      _resetPipelines();
      _resetShadowmapProvider();
      _updateShadowmapProvider();
    }

    inline float DirectLight::blurSize() const noexcept
    {
      return _blurSize;
    }

    inline void DirectLight::setBlurSize(float newValue) noexcept
    {
      _blurSize = newValue;
    }

    inline float DirectLight::blurRelativeRadius() const noexcept
    {
      return (blurSize() / 2.f) / (2.f * radius());
    }

    inline Sampler* DirectLight::shadowmapSampler() noexcept
    {
      return _shadowmapSampler.get();
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