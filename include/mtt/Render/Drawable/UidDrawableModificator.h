#pragma once

#include <mtt/Render/Drawable/Drawable.h>
#include <mtt/Utilities/UID.h>

namespace mtt
{
  class UidDrawableModificator : public Drawable::Modificator
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
                      Modificator** next,
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
