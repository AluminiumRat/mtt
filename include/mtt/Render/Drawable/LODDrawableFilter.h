#pragma once

#include <mtt/Render/Drawable/Drawable.h>

namespace mtt
{
  class LODDrawableFilter : public Drawable::Modificator
  {
  public:
    LODDrawableFilter();
    LODDrawableFilter(const LODDrawableFilter&) = delete;
    LODDrawableFilter& operator = (const LODDrawableFilter&) = delete;
    virtual ~LODDrawableFilter() noexcept = default;

    /// Minimal meter per pixel rate
    inline float minMppx() const noexcept;
    /// Maximal meter per pixel rate
    inline float maxMppx() const noexcept;
    void setRange(float minMppx, float maxMppx) noexcept;

    virtual void draw(DrawPlanBuildInfo& buildInfo,
                      Modificator** next,
                      size_t modifiactorsLeft,
                      Drawable& drawable) const override;
  private:
    float _minMppx;
    float _maxMppx;
  };

  inline float LODDrawableFilter::minMppx() const noexcept
  {
    return _minMppx;
  }

  inline float LODDrawableFilter::maxMppx() const noexcept
  {
    return _maxMppx;
  }
}
