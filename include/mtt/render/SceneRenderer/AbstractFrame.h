#pragma once

#include <glm/vec2.hpp>

namespace mtt
{
  class AbstractFrameBuilder;
  class ImageView;

  /// This is collection of frame-related data (frame buffers for example).
  /// Specific frames are created by specific frame renderers.
  class AbstractFrame
  {
  protected:
    inline AbstractFrame( const glm::uvec2& extent,
                          AbstractFrameBuilder& builder) noexcept;

  public:
    AbstractFrame(const AbstractFrame&) = delete;
    AbstractFrame& operator = (const AbstractFrame&) = delete;
    virtual ~AbstractFrame() noexcept = default;

    inline const glm::uvec2& extent() const noexcept;
    inline AbstractFrameBuilder& builder() const noexcept;

  private:
    glm::uvec2 _extent;
    AbstractFrameBuilder& _builder;
  };

  inline AbstractFrame::AbstractFrame(const glm::uvec2& extent,
                                      AbstractFrameBuilder& builder) noexcept :
    _extent(extent),
    _builder(builder)
  {
  }

  inline const glm::uvec2& AbstractFrame::extent() const noexcept
  {
    return _extent;
  }

  inline AbstractFrameBuilder& AbstractFrame::builder() const noexcept
  {
    return _builder;
  }
}