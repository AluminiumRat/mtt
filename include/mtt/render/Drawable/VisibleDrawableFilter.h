#pragma once

#include <mtt/render/Drawable/Drawable.h>

namespace mtt
{
  class VisibleDrawableFilter : public Drawable::Modificator
  {
  public:
    VisibleDrawableFilter();
    VisibleDrawableFilter(const VisibleDrawableFilter&) = delete;
    VisibleDrawableFilter& operator = (const VisibleDrawableFilter&) = delete;
    virtual ~VisibleDrawableFilter() noexcept = default;

    inline bool visible() const noexcept;
    virtual void setVisible(bool newValue) noexcept;

    virtual void draw(DrawPlanBuildInfo& buildInfo,
                      Modificator** next,
                      size_t modifiactorsLeft,
                      Drawable& drawable) const override;

  private:
    bool _visible;
  };

  inline bool VisibleDrawableFilter::visible() const noexcept
  {
    return _visible;
  }
}
