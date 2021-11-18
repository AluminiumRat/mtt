#pragma once

#include <glm/vec2.hpp>

#include <mtt/Render/Pipeline/VolatileUniform.h>
#include <mtt/Render/RenderPass/AbstractFilter.h>

namespace mtt
{
  /// This is subsidiary render pass that used in ShadowmapBuilder.
  class ShadowmapBlurFilter : public AbstractFilter
  {
  public:
    ShadowmapBlurFilter(VkFormat targetFormat,
                        VkImageLayout targetLayout,
                        LogicalDevice& device);
    ShadowmapBlurFilter(const ShadowmapBlurFilter&) = delete;
    ShadowmapBlurFilter& operator = (const ShadowmapBlurFilter&) = delete;
  protected:
    virtual ~ShadowmapBlurFilter() = default;

  public:
    inline const glm::ivec2& shift() const noexcept;
    inline void setShift(const glm::ivec2& newValue) noexcept;

    inline float weight() const noexcept;
    inline void setWeight(float newValue) noexcept;

  private:
    struct BlurParameters
    {
      alignas(4) glm::ivec2 shift;
      alignas(4) float weight;
    };
    BlurParameters _blurParameters;
    VolatileUniform<BlurParameters> _parametersUniform;
  };

  inline const glm::ivec2& ShadowmapBlurFilter::shift() const noexcept
  {
    return _blurParameters.shift;
  }

  inline void ShadowmapBlurFilter::setShift(const glm::ivec2& newValue) noexcept
  {
    _blurParameters.shift = newValue;
  }

  inline float ShadowmapBlurFilter::weight() const noexcept
  {
    return _blurParameters.weight;
  }

  inline void ShadowmapBlurFilter::setWeight(float newValue) noexcept
  {
    _blurParameters.weight = newValue;
  }
};