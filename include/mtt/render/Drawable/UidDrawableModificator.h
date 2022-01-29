#pragma once

#include <mtt/render/Drawable/DrawableModificator.h>
#include <mtt/utilities/UID.h>

namespace mtt
{
  class UidDrawableModificator : public DrawableModificator
  {
  public:
    UidDrawableModificator() = default;
    UidDrawableModificator(const UidDrawableModificator&) = delete;
    UidDrawableModificator& operator = (
                                      const UidDrawableModificator&) = delete;
    virtual ~UidDrawableModificator() noexcept = default;

    inline const UID& uid() const noexcept;
    virtual void setUid(UID newValue) noexcept;

    virtual void draw(DrawPlanBuildInfo& buildInfo,
                      DrawableModificator** next,
                      size_t modifiactorsLeft,
                      Drawable& drawable) const override;

  private:
    UID _uid;
  };

  inline const UID& UidDrawableModificator::uid() const noexcept
  {
    return _uid;
  }
}
