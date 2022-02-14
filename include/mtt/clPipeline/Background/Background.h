#pragma once

#include <memory>

#include <glm/vec3.hpp>

#include <mtt/clPipeline/Background/BackgroundAreaModificator.h>
#include <mtt/clPipeline/Background/BackgroundDrawable.h>
#include <mtt/clPipeline/Background/BackgroundProperties.h>
#include <mtt/render/Pipeline/CubeTexture.h>
#include <mtt/render/Pipeline/Sampler.h>
#include <mtt/render/SceneGraph/ProtectedGroupNode.h>
#include <mtt/render/SceneGraph/SimpleDrawableNode.h>
#include <mtt/render/CompositeRenderObject.h>

namespace mtt
{
  class LogicalDevice;

  namespace clPipeline
  {
    class Background :  public CompositeRenderObject,
                        public ProtectedGroupNode
    {
    public:
      explicit Background(LogicalDevice& device);
      Background(const Background&) = delete;
      Background& operator = (const Background&) = delete;
      virtual ~Background() = default;

      inline const glm::vec3& luminance() const noexcept;
      inline void setLuminance(const glm::vec3 & newValue) noexcept;

      inline float dissolutionStartDistance() const noexcept;
      inline void setDissolutionStartDistance(float newValue) noexcept;

      inline float dissolutionLength() const noexcept;
      inline void setDissolutionLength(float newValue) noexcept;

      void setLuminanceTexture(std::shared_ptr<CubeTexture> texture) noexcept;
      inline const CubeTexture* luminanceTexture() const noexcept;
      inline CubeTexture* luminanceTexture() noexcept;

      inline BackgroundAreaModificator& areaModificator() noexcept;
      inline const BackgroundAreaModificator& areaModificator() const noexcept;

      inline BackgroundDrawable& drawable() noexcept;
      inline const BackgroundDrawable& drawable() const noexcept;

      virtual size_t areaModificatorsNumber() const noexcept override;
      virtual AreaModificator& areaModificator(size_t index) noexcept override;

      virtual size_t unculledDrawablesNumber() const noexcept override;
      virtual Drawable& unculledDrawable(size_t index) noexcept override;

    private:
      friend class BackgroundAreaModificator;
      friend class BackgroundDrawable;

    private:
      BackgroundProperties _properties;
      Sampler _luminanceSampler;
      BackgroundAreaModificator _areaModificator;
      BackgroundDrawable _drawable;
      mtt::SimpleDrawableNode _drawableNode;
    };

    inline const glm::vec3& Background::luminance() const noexcept
    {
      return _properties.luminance;
    }

    inline void Background::setLuminance(const glm::vec3& newValue) noexcept
    {
      _properties.luminance = newValue;
    }

    inline float Background::dissolutionStartDistance() const noexcept
    {
      return _properties.dissolutionStartDistance;
    }

    inline void Background::setDissolutionStartDistance(float newValue) noexcept
    {
      _properties.dissolutionStartDistance = newValue;
    }

    inline float Background::dissolutionLength() const noexcept
    {
      return _properties.dissolutionLength;
    }

    inline void Background::setDissolutionLength(float newValue) noexcept
    {
      _properties.dissolutionLength = newValue;
    }

    inline const CubeTexture* Background::luminanceTexture() const noexcept
    {
      return static_cast<const CubeTexture*>(
                                          _luminanceSampler.attachedTexture(0));
    }

    inline CubeTexture* Background::luminanceTexture() noexcept
    {
      return static_cast<CubeTexture*>(_luminanceSampler.attachedTexture(0));
    }

    inline BackgroundAreaModificator& Background::areaModificator() noexcept
    {
      return _areaModificator;
    }

    inline const BackgroundAreaModificator&
                                  Background::areaModificator() const noexcept
    {
      return _areaModificator;
    }

    inline BackgroundDrawable& Background::drawable() noexcept
    {
      return _drawable;
    }

    inline const BackgroundDrawable& Background::drawable() const noexcept
    {
      return _drawable;
    }
  }
}