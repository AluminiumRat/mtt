#pragma once

#include <glm/mat4x4.hpp>

#include <mtt/render/Drawable/DrawableModificator.h>

namespace mtt
{
  class CameraNode;
  struct ViewInfo;

  /// This modificator is used to remove scale and slope of local to view
  /// matrix on the fly.
  /// Warning!!! The bounding sphere of drawable node is not valid. Do not use
  /// culling with this modificator.
  class AntiscaleDrawableModificator : public DrawableModificator
  {
  public:
    AntiscaleDrawableModificator();
    AntiscaleDrawableModificator(const AntiscaleDrawableModificator&) = delete;
    AntiscaleDrawableModificator& operator = (
                                  const AntiscaleDrawableModificator&) = delete;
    virtual ~AntiscaleDrawableModificator() noexcept = default;

    virtual void draw(DrawPlanBuildInfo& buildInfo,
                      DrawableModificator** next,
                      size_t modifiactorsLeft,
                      Drawable& drawable) const override;

    static glm::mat4 getCutedTransform(const glm::mat4& origin) noexcept;
  };
}
