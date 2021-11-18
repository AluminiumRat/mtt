#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <mtt/Render/Drawable/Drawable.h>

namespace mtt
{
  class RotateToCameraModificator : public Drawable::Modificator
  {
  public:
    RotateToCameraModificator() = default;
    RotateToCameraModificator(const RotateToCameraModificator&) = delete;
    RotateToCameraModificator& operator = (
                                    const RotateToCameraModificator&) = delete;
    virtual ~RotateToCameraModificator() noexcept = default;

    virtual void draw(DrawPlanBuildInfo& buildInfo,
                      Modificator** next,
                      size_t modifiactorsLeft,
                      Drawable& drawable) const override;
  };
}
