#pragma once

#include <mtt/render/Drawable/Drawable.h>

namespace mtt
{
  class SelectionDrawableModificator : public Drawable::Modificator
  {
  public:
    SelectionDrawableModificator();
    SelectionDrawableModificator(const SelectionDrawableModificator&) = delete;
    SelectionDrawableModificator& operator = (
                                  const SelectionDrawableModificator&) = delete;
    virtual ~SelectionDrawableModificator() noexcept = default;

    inline bool selected() const noexcept;
    virtual void setSelected(bool newValue) noexcept;

    virtual void draw(DrawPlanBuildInfo& buildInfo,
                      Modificator** next,
                      size_t modifiactorsLeft,
                      Drawable& drawable) const override;

  private:
    bool _selected;
  };

  inline bool SelectionDrawableModificator::selected() const noexcept
  {
    return _selected;
  }
}
