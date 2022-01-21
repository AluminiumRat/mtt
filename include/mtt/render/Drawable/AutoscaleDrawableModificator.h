#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <mtt/render/Drawable/Drawable.h>

namespace mtt
{
  class CameraNode;
  struct ViewInfo;

  /// This modificator is used to scale drawable on the fly.
  /// Drawable are drawn at the same size regardless of distance.
  /// Warning!!! The bounding sphere of drawable node is not valid. Do not use
  /// culling with this modificator.
  class AutoscaleDrawableModificator : public Drawable::Modificator
  {
  public:
    enum Mode
    {
      NO_SCALE_MODE,
      VIEWPORT_SCALE_MODE,
      PIXEL_SCALE_MODE
    };

  public:
    AutoscaleDrawableModificator();
    AutoscaleDrawableModificator(const AutoscaleDrawableModificator&) = delete;
    AutoscaleDrawableModificator& operator = (
                                  const AutoscaleDrawableModificator&) = delete;
    virtual ~AutoscaleDrawableModificator() noexcept = default;

    inline Mode mode() const noexcept;
    inline void setMode(Mode newValue) noexcept;

    virtual void draw(DrawPlanBuildInfo& buildInfo,
                      Modificator** next,
                      size_t modifiactorsLeft,
                      Drawable& drawable) const override;

    /// position is in the view space
    float static scale( const glm::vec3& position,
                        const ViewInfo& viewInfo,
                        Mode mode) noexcept;

  private:
    Mode _mode;
  };

  inline AutoscaleDrawableModificator::Mode
                            AutoscaleDrawableModificator::mode() const noexcept
  {
    return _mode;
  }

  inline void AutoscaleDrawableModificator::setMode(Mode newValue) noexcept
  {
    _mode = newValue;
  }
}
