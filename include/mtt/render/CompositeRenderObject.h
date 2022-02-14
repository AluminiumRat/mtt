#pragma once

namespace mtt
{
  class AreaModificator;
  class Drawable;
  class DrawableNode;

  class CompositeRenderObject
  {
  public:
    CompositeRenderObject() = default;
    CompositeRenderObject(const CompositeRenderObject&) = delete;
    CompositeRenderObject& operator = (const CompositeRenderObject&) = delete;
    virtual ~CompositeRenderObject() noexcept = default;

    virtual size_t culledDrawablesNumber() const noexcept;
    virtual DrawableNode& culledDrawable(size_t index) noexcept;

    virtual size_t areaModificatorsNumber() const noexcept;
    virtual AreaModificator& areaModificator(size_t index) noexcept;

    virtual size_t unculledDrawablesNumber() const noexcept;
    virtual Drawable& unculledDrawable(size_t index) noexcept;

    virtual size_t hudDrawablesNumber() const noexcept;
    virtual Drawable& hudDrawable(size_t index) noexcept;
  };
}