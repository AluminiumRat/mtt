#pragma once

#include <mtt/render/Drawable/DrawableModificator.h>

namespace mtt
{
  class VisibleDrawableFilter : public DrawableModificator
  {
  public:
    VisibleDrawableFilter();
    VisibleDrawableFilter(const VisibleDrawableFilter&) = delete;
    VisibleDrawableFilter& operator = (const VisibleDrawableFilter&) = delete;
    virtual ~VisibleDrawableFilter() noexcept = default;

    inline bool visible() const noexcept;
    virtual void setVisible(bool newValue) noexcept;

    virtual void draw(DrawPlanBuildInfo& buildInfo,
                      DrawableModificator** next,
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
