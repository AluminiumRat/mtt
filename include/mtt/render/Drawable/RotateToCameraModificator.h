#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <mtt/render/Drawable/DrawableModificator.h>

namespace mtt
{
  class RotateToCameraModificator : public DrawableModificator
  {
  public:
    RotateToCameraModificator() = default;
    RotateToCameraModificator(const RotateToCameraModificator&) = delete;
    RotateToCameraModificator& operator = (
                                    const RotateToCameraModificator&) = delete;
    virtual ~RotateToCameraModificator() noexcept = default;

    virtual void draw(DrawPlanBuildInfo& buildInfo,
                      DrawableModificator** next,
                      size_t modifiactorsLeft,
                      Drawable& drawable) const override;
  };
}
