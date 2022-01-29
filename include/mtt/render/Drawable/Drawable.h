#pragma once

#include <vector>

namespace mtt
{
  class AreaModificatorSet;
  class DrawableModificator;
  struct DrawPlanBuildInfo;

  class Drawable
  {
  public:
    Drawable() noexcept = default;
    Drawable(const Drawable&) noexcept = default;
    Drawable& operator = (const Drawable&) noexcept = default;
    virtual ~Drawable() noexcept = default;

    virtual void addToDrawPlan(DrawPlanBuildInfo& buildInfo);

    virtual void addModificator(DrawableModificator& modificator);
    virtual void removeModificator(DrawableModificator& modificator) noexcept;

    virtual void registerAreaModificators(AreaModificatorSet& set);
    virtual void unregisterAreaModificators(AreaModificatorSet& set) noexcept;

  protected:
    friend class DrawableModificator;
    virtual void buildDrawActions(DrawPlanBuildInfo& buildInfo) = 0;

  private:
    using Modificators = std::vector<DrawableModificator*>;
    Modificators _modificators;
  };
}