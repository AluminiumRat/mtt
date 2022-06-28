#pragma once

#include <memory>
#include <optional>

#include <mtt/clPipeline/Lighting/CubeShadowmapProvider.h>
#include <mtt/clPipeline/Lighting/PointLightApplicator.h>
#include <mtt/clPipeline/Lighting/PointLightAreaModificator.h>
#include <mtt/clPipeline/Lighting/PointLightData.h>
#include <mtt/render/Pipeline/Buffer.h>
#include <mtt/render/Pipeline/CubeTexture.h>
#include <mtt/render/Pipeline/Sampler.h>
#include <mtt/render/SceneGraph/CameraNode.h>
#include <mtt/render/SceneGraph/CompositeObjectNode.h>
#include <mtt/utilities/Sphere.h>

namespace mtt
{
  class AbstractField;
  struct DrawPlanBuildInfo;

  namespace clPipeline
  {
    class PointLight : public CompositeObjectNode
    {
    public:
      PointLight( bool forwardLightingEnabled,
                  bool defferedLightingEnabled,
                  LogicalDevice& device);
      PointLight(const PointLight&) = delete;
      PointLight& operator = (const PointLight&) = delete;
      virtual ~PointLight() = default;

      inline const glm::vec3& illuminance() const noexcept;
      inline void setIlluminance(const glm::vec3& newValue) noexcept;

      inline float distance() const noexcept;
      inline void setDistance(float newValue) noexcept;

      inline CubeTexture* filterTexture() const noexcept;
      /// You can set nullptr to remove filter texture
      void setFilterTexture(std::shared_ptr<CubeTexture> newTexture);

      inline const CameraNode& shadowmapFrontCamera() const noexcept;

      inline unsigned int shadowmapExtent() const noexcept;
      inline void setShadowmapExtent(unsigned int newValue);

      inline AbstractField* shadowmapField() const noexcept;
      inline void setShadowmapField(AbstractField* newField);

      inline float blurAngle() const noexcept;
      inline void setBlurAngle(float newValue) noexcept;

      inline Sphere getBoundSphere() const noexcept;

      virtual size_t culledDrawablesNumber() const noexcept override;
      virtual DrawableNode& culledDrawable(size_t index) noexcept override;

      virtual size_t areaModificatorsNumber() const noexcept override;
      virtual AreaModificator& areaModificator(size_t index) noexcept override;

    private:
      friend class PointLightApplicator;
      friend class PointLightAreaModificator;
      PointLightData buildDrawData(
                            const DrawPlanBuildInfo& buildInfo) const noexcept;
      inline Sampler* filterSampler() noexcept;
      inline CubeShadowmapProvider* shadowmapProvider() const noexcept;
      inline Sampler* shadowmapSampler() noexcept;
      inline Buffer* blurShiftsBuffer() noexcept;

    private:
      void _resetShadowmapProvider() noexcept;
      void _updateShadowmapProvider();
      void _updateBound() noexcept;
      void _updateShadowmapCamera() noexcept;
      void _resetPipelines() noexcept;
      void _buildBlurShifts();

    private:
      LogicalDevice& _device;

      CameraNode _shadowmapFrontCamera;
      std::unique_ptr<CubeShadowmapProvider> _shadowmapProvider;
      unsigned int _shadowmapExtent;
      AbstractField* _shadowmapField;
      std::unique_ptr<Sampler> _shadowmapSampler;

      std::optional<Buffer> _blurShiftsBuffer;
      std::vector<uint32_t> _startShifts;

      glm::vec3 _illuminance;
      float _distance;
      float _blurAngle;

      std::optional<Sampler> _filterSampler;

      std::unique_ptr<PointLightApplicator> _defferedLightApplicator;
      std::unique_ptr<PointLightAreaModificator> _forwardLightApplicator;
    };

    inline const glm::vec3& PointLight::illuminance() const noexcept
    {
      return _illuminance;
    }

    inline void PointLight::setIlluminance(const glm::vec3& newValue) noexcept
    {
      glm::vec3 newIlluminance = glm::max(glm::vec3(0.f), newValue);
      _illuminance = newIlluminance;
    }

    inline float PointLight::distance() const noexcept
    {
      return _distance;
    }

    inline void PointLight::setDistance(float newValue) noexcept
    {
      newValue = glm::max(0.f, newValue);
      if (_distance == newValue) return;
      _distance = newValue;
      _updateShadowmapCamera();
      _updateBound();
    }

    inline CubeTexture* PointLight::filterTexture() const noexcept
    {
      if(!_filterSampler.has_value()) return nullptr;
      return static_cast<CubeTexture*>(_filterSampler->attachedTexture(0));
    }

    inline const CameraNode& PointLight::shadowmapFrontCamera() const noexcept
    {
      return _shadowmapFrontCamera;
    }

    inline CubeShadowmapProvider* PointLight::shadowmapProvider() const noexcept
    {
      return _shadowmapProvider.get();
    }

    inline unsigned int PointLight::shadowmapExtent() const noexcept
    {
      return _shadowmapExtent;
    }

    inline void PointLight::setShadowmapExtent(unsigned int newValue)
    {
      if(_shadowmapExtent == newValue) return;
      _shadowmapExtent = newValue;
      _updateShadowmapProvider();
      if (_shadowmapProvider != nullptr)
      {
        try
        {
          _shadowmapProvider->setFrameExtent(_shadowmapExtent);
        }
        catch (...)
        {
          _resetShadowmapProvider();
          throw;
        }
      }
    }

    inline AbstractField* PointLight::shadowmapField() const noexcept
    {
      return _shadowmapField;
    }

    inline void PointLight::setShadowmapField(AbstractField* newField)
    {
      if(_shadowmapField == newField) return;
      _shadowmapField = newField;
      _updateShadowmapProvider();
      if (_shadowmapProvider != nullptr)
      {
        _shadowmapProvider->setTargetField(_shadowmapField);
      }
    }

    inline float PointLight::blurAngle() const noexcept
    {
      return _blurAngle;
    }

    inline void PointLight::setBlurAngle(float newValue) noexcept
    {
      newValue = glm::clamp(newValue, 0.f, glm::pi<float>() / 10.f);
      _blurAngle = newValue;
    }

    inline Sphere PointLight::getBoundSphere() const noexcept
    {
      return Sphere(glm::vec3(0.f, 0.f, 0.f), distance());
    }

    inline Sampler* PointLight::filterSampler() noexcept
    {
      if(_filterSampler.has_value()) return &_filterSampler.value();
      return nullptr;
    }

    inline Sampler* PointLight::shadowmapSampler() noexcept
    {
      return _shadowmapSampler.get();
    }

    inline Buffer* PointLight::blurShiftsBuffer() noexcept
    {
      if(_blurShiftsBuffer.has_value())  return &_blurShiftsBuffer.value();
      else return nullptr;
    }
  }
}