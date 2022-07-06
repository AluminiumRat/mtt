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
  class CameraNode;
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

      inline float height() const noexcept;
      inline void setHeight(float newValue) noexcept;

      inline const glm::vec3& direction() const noexcept;
      inline void setDirection(const glm::vec3& newValue) noexcept;

      inline float shadowDistance() const noexcept;
      inline void setShadowDistance(float newValue);

      inline AbstractField* shadowmapField() const noexcept;
      inline void setShadowmapField(AbstractField* newField);

      inline unsigned int opaqueShadowmapExtent() const noexcept;
      inline unsigned int transparentShadowmapExtent() const noexcept;
      inline void setShadowmapExtent( unsigned int opaqueShadowmapExtent,
                                      unsigned int transparentShadowmapExtent);

      inline size_t cascadeSize() const noexcept;
      inline void setCascadeSize(size_t newValue);

      inline float blurSize() const noexcept;
      inline void setBlurSize(float newValue) noexcept;

      virtual size_t unculledDrawablesNumber() const noexcept override;
      virtual Drawable& unculledDrawable(size_t index) noexcept override;

      virtual size_t areaModificatorsNumber() const noexcept override;
      virtual AreaModificator& areaModificator(size_t index) noexcept override;

    private:
      friend class DirectLightApplicator;
      friend class DirectLightAreaModificator;
      DirectLightData buildDrawData(
                            const DrawPlanBuildInfo& buildInfo) const noexcept;
      inline CascadeShadowMapProvider* shadowmapProvider() noexcept;
      void updateCameras( CameraNode& shadowmapCamera,
                          CameraNode& viewCamera,
                          const DrawPlanBuildInfo& buildInfo) const noexcept;
      inline Sampler* opaqueShadowmapSampler() noexcept;
      inline Sampler* transparentShadowmapSampler() noexcept;

      /// Blur radius in UV coordinates, i.e. blur radius relative to light
      /// diameter
      inline float blurRelativeRadius(float coordMultiplicator) const noexcept;

    private:
      void _resetShadowmapProvider() noexcept;
      void _updateShadowmapProvider();
      void _updateShadowmapProjection() noexcept;
      void _resetPipelines() noexcept;
      glm::mat4 _getShiftMatrix(
                            const DrawPlanBuildInfo& buildInfo) const noexcept;

    private:
      LogicalDevice& _device;

      glm::mat4 _shadowmapProjectionMatrix;
      std::unique_ptr<CascadeShadowMapProvider> _shadowmapProvider;
      unsigned int _opaqueShadowmapExtent;
      unsigned int _transparentShadowmapExtent;
      AbstractField* _shadowmapField;
      std::unique_ptr<Sampler> _opaqueShadowmapSampler;
      std::unique_ptr<Sampler> _transparentShadowmapSampler;

      glm::vec3 _illuminance;
      float _height;
      glm::vec3 _direction;
      float _shadowDistance;
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

    inline float DirectLight::height() const noexcept
    {
      return _height;
    }

    inline void DirectLight::setHeight(float newValue) noexcept
    {
      if(_height == newValue) return;
      _height = newValue;
      _updateShadowmapProjection();
    }

    inline const glm::vec3& DirectLight::direction() const noexcept
    {
      return _direction;
    }

    inline void DirectLight::setDirection(const glm::vec3& newValue) noexcept
    {
      glm::vec3 newDirection = newValue;
      if (newDirection.x == 0.f &&
          newDirection.y == 0.f &&
          newDirection.z == 0.f)
      {
        newDirection = glm::vec3(0.f, 0.f, -1.f);
      }
      else newDirection = glm::normalize(newDirection);

      if (_direction == newDirection) return;
      _direction = newDirection;
      _updateShadowmapProjection();
    }

    inline float DirectLight::shadowDistance() const noexcept
    {
      return _shadowDistance;
    }

    inline void DirectLight::setShadowDistance(float newValue)
    {
      newValue = glm::max(0.f, newValue);
      if(_shadowDistance == newValue) return;

      _shadowDistance = newValue;
      _updateShadowmapProjection();
      _updateShadowmapProvider();
    }

    inline CascadeShadowMapProvider* DirectLight::shadowmapProvider() noexcept
    {
      return _shadowmapProvider.get();
    }

    inline unsigned int DirectLight::opaqueShadowmapExtent() const noexcept
    {
      return _opaqueShadowmapExtent;
    }

    inline unsigned int DirectLight::transparentShadowmapExtent() const noexcept
    {
      return _transparentShadowmapExtent;
    }

    inline void DirectLight::setShadowmapExtent(
                                        unsigned int opaqueShadowmapExtent,
                                        unsigned int transparentShadowmapExtent)
    {
      if( _opaqueShadowmapExtent == opaqueShadowmapExtent &&
          _transparentShadowmapExtent == transparentShadowmapExtent) return;

      _opaqueShadowmapExtent = opaqueShadowmapExtent;
      _transparentShadowmapExtent = transparentShadowmapExtent;

      _updateShadowmapProvider();
      if (_shadowmapProvider != nullptr)
      {
        try
        {
          _shadowmapProvider->setMapExtent(
                                      glm::uvec2(_opaqueShadowmapExtent),
                                      glm::uvec2(_transparentShadowmapExtent));
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

    inline float DirectLight::blurRelativeRadius(
                                        float coordMultiplicator) const noexcept
    {
      return blurSize() / shadowDistance() / 4.f * coordMultiplicator +
                                                  .5f / _opaqueShadowmapExtent;
    }

    inline Sampler* DirectLight::opaqueShadowmapSampler() noexcept
    {
      return _opaqueShadowmapSampler.get();
    }

    inline Sampler* DirectLight::transparentShadowmapSampler() noexcept
    {
      return _transparentShadowmapSampler.get();
    }
  }
}