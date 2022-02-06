#pragma once

#include <mtt/clPipeline/Lighting/AbstractLight.h>
#include <mtt/clPipeline/Lighting/AmbientLightApplicator.h>
#include <mtt/clPipeline/Lighting/AmbientLightAreaModificator.h>
#include <mtt/clPipeline/Lighting/AmbientLightData.h>
#include <mtt/render/Pipeline/Sampler.h>
#include <mtt/render/Pipeline/CubeTexture.h>

namespace mtt
{
  class LogicalDevice;

  namespace clPipeline
  {
    class AmbientLight : public AbstractLight
    {
    public:
      explicit AmbientLight(LogicalDevice& device);
      AmbientLight(const AmbientLight&) = delete;
      AmbientLight& operator = (const AmbientLight&) = delete;

      virtual DrawableNode* defferedLightApplicator() noexcept override;
      virtual AreaModificator* forwardLightModificator() noexcept override;

      inline bool infinityAreaMode() const noexcept;
      inline void setInfinityAreaMode(bool newValue) noexcept;

      inline const glm::vec3& illuminance() const noexcept;
      inline void setIlluminance(const glm::vec3& newValue) noexcept;

      inline float distance() const noexcept;
      inline void setDistance(float newValue) noexcept;

      inline float saturationDistance() const noexcept;
      inline void setSaturationDistance(float newValue) noexcept;

      inline CubeTexture* ambientMap() const noexcept;
      void setAmbientMap(std::shared_ptr<CubeTexture> newMap) noexcept;

      inline CubeTexture* diffuseLuminanceMap() const noexcept;
      void setDiffuseLuminanceMap(std::shared_ptr<CubeTexture> newMap) noexcept;

    private:
      void _updateBound() noexcept;
      void _resetPipelines() noexcept;

    private:
      AmbientLightData _lightData;

      Sampler _ambientMapSampler;
      CubeTexture* _ambientMap;

      Sampler _diffuseLuminanceSampler;
      CubeTexture* _diffuseLuminanceMap;

      AmbientLightApplicator _defferedApplicator;
      AmbientLightAreaModificator _forwardApplicator;
    };

    inline bool AmbientLight::infinityAreaMode() const noexcept
    {
      return _lightData.infinityAreaMode;
    }

    inline void AmbientLight::setInfinityAreaMode(bool newValue) noexcept
    {
      if(_lightData.infinityAreaMode == newValue) return;
      _lightData.infinityAreaMode = newValue;
      _resetPipelines();
    }

    inline const glm::vec3& AmbientLight::illuminance() const noexcept
    {
      return _lightData.illuminance;
    }

    inline void AmbientLight::setIlluminance(
                                            const glm::vec3& newValue) noexcept
    {
      _lightData.illuminance = newValue;
    }

    inline float AmbientLight::distance() const noexcept
    {
      return _lightData.distance;
    }

    inline void AmbientLight::setDistance(float newValue) noexcept
    {
      if(_lightData.distance == newValue) return;
      _lightData.distance = newValue;
      _updateBound();
    }

    inline float AmbientLight::saturationDistance() const noexcept
    {
      return _lightData.saturationDistance;
    }

    inline void AmbientLight::setSaturationDistance(float newValue) noexcept
    {
      _lightData.saturationDistance = newValue;
    }

    inline CubeTexture* AmbientLight::ambientMap() const noexcept
    {
      return _ambientMap;
    }

    inline CubeTexture* AmbientLight::diffuseLuminanceMap() const noexcept
    {
      return _diffuseLuminanceMap;
    }
  }
}