#pragma once

#include <memory>
#include <optional>

#include <glm/vec2.hpp>

#include <mtt/clPipeline/Lighting/SpotLightApplicator.h>
#include <mtt/clPipeline/Lighting/SpotLightAreaModificator.h>
#include <mtt/clPipeline/Lighting/SpotLightData.h>
#include <mtt/clPipeline/Lighting/ShadowMapProvider.h>
#include <mtt/render/Pipeline/Sampler.h>
#include <mtt/render/Pipeline/Texture2D.h>
#include <mtt/render/SceneGraph/CameraNode.h>
#include <mtt/render/SceneGraph/CompositeObjectNode.h>
#include <mtt/utilities/Sphere.h>

namespace mtt
{
  class AbstractField;
  struct DrawPlanBuildInfo;

  namespace clPipeline
  {
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
      inline void setAngle(float newValue);

      inline Texture2D* filterTexture() const noexcept;
      /// You can set nullptr to remove filter texture
      void setFilterTexture(std::shared_ptr<Texture2D> newTexture);

      inline const CameraNode& shadowmapCamera() const noexcept;

      inline unsigned int shadowmapExtent() const noexcept;
      inline void setShadowmapExtent(unsigned int newValue);

      inline AbstractField* shadowmapField() const noexcept;
      inline void setShadowmapField(AbstractField* newField);

      inline float blurAngle() const noexcept;
      inline void setBlurAngle(float newValue);

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
      inline ShadowMapProvider* shadowMapProvider() const noexcept;

    private:
      void _resetShadowmapProvider() noexcept;
      void _updateShadowmapProvider();
      void _updateShadowmapCamera() noexcept;
      void _updateBound() noexcept;
      void _resetPipelines() noexcept;

    private:
      LogicalDevice& _device;

      CameraNode _shadowmapCamera;
      std::unique_ptr<ShadowMapProvider> _shadowMapProvider;
      unsigned int _shadowmapExtent;
      AbstractField* _shadowmapField;
      std::unique_ptr<Sampler> _shadowmapSampler;

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

    inline void SpotLight::setAngle(float newValue)
    {
      newValue = glm::clamp(newValue, 0.f, glm::pi<float>() / 2.f);
      if (_angle == newValue) return;
      _angle = newValue;
      _updateBound();
      _updateShadowmapCamera();

      try
      {
        _updateShadowmapProvider();
      }
      catch (...)
      {
        _resetShadowmapProvider();
        throw;
      }
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
      return _shadowMapProvider.get();
    }

    inline unsigned int SpotLight::shadowmapExtent() const noexcept
    {
      return _shadowmapExtent;
    }

    inline void SpotLight::setShadowmapExtent(unsigned int newValue)
    {
      if(_shadowmapExtent == newValue) return;
      _shadowmapExtent = newValue;
      _resetPipelines();
      _updateShadowmapProvider();
      if (_shadowMapProvider != nullptr)
      {
        try
        {
          _shadowMapProvider->setFrameExtent(glm::uvec2(_shadowmapExtent));
        }
        catch (...)
        {
          _resetShadowmapProvider();
          throw;
        }
      }
    }
    inline AbstractField* SpotLight::shadowmapField() const noexcept
    {
      return _shadowmapField;
    }

    inline void SpotLight::setShadowmapField(AbstractField* newField)
    {
      if(_shadowmapField == newField) return;
      _shadowmapField = newField;
      _updateShadowmapProvider();
      if (_shadowMapProvider != nullptr)
      {
        _shadowMapProvider->setTargetField(_shadowmapField);
      }
    }

    inline float SpotLight::blurAngle() const noexcept
    {
      return _blurAngle;
    }

    inline void SpotLight::setBlurAngle(float newValue)
    {
      newValue = glm::clamp(newValue, 0.f, glm::pi<float>() / 10.f);
      _blurAngle = newValue;
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
      return _shadowmapSampler.get();
    }

  }
}