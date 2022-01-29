#pragma once

#include <mtt/render/Drawable/Drawable.h>

namespace mtt
{
  class DrawableModificator
  {
  public:
    DrawableModificator() = default;
    DrawableModificator(const DrawableModificator&) = delete;
    DrawableModificator& operator = (const DrawableModificator&) = delete;
    virtual ~DrawableModificator() noexcept = default;

    virtual void draw(DrawPlanBuildInfo& buildInfo,
                      DrawableModificator** next,
                      size_t modifiactorsLeft,
                      Drawable& drawable) const = 0;
  protected:
    inline void drawNext( DrawPlanBuildInfo& buildInfo,
                          DrawableModificator** next,
                          size_t modifiactorsLeft,
                          Drawable& drawable) const;
  };

  inline void DrawableModificator::drawNext(DrawPlanBuildInfo& buildInfo,
                                            DrawableModificator** next,
                                            size_t modifiactorsLeft,
                                            Drawable& drawable) const
  {
    if(modifiactorsLeft != 0)
    {
      (*next)->draw(buildInfo, next + 1, modifiactorsLeft - 1, drawable);
    }
    else drawable.buildDrawActions(buildInfo);
  }
}