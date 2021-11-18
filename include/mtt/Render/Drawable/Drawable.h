#pragma once

#include <vector>

namespace mtt
{
  struct DrawPlanBuildInfo;

  class Drawable
  {
  public:
    class Modificator
    {
    public:
      Modificator() = default;
      Modificator(const Modificator&) = delete;
      Modificator& operator = (const Modificator&) = delete;
      virtual ~Modificator() noexcept = default;

      virtual void draw(DrawPlanBuildInfo& buildInfo,
                        Modificator** next,
                        size_t modifiactorsLeft,
                        Drawable& drawable) const = 0;
    protected:
      inline void drawNext( DrawPlanBuildInfo& buildInfo,
                            Modificator** next,
                            size_t modifiactorsLeft,
                            Drawable& drawable) const;
    };

  public:
    Drawable() noexcept = default;
    Drawable(const Drawable&) noexcept = default;
    Drawable& operator = (const Drawable&) noexcept = default;
    virtual ~Drawable() noexcept = default;

    inline virtual void addToDrawPlan(DrawPlanBuildInfo& buildInfo);

    void registedModificator(Modificator& modificator);
    void unregistedModificator(Modificator& modificator) noexcept;

  protected:
    virtual void buildDrawActions(DrawPlanBuildInfo& buildInfo) = 0;

  private:
    using Modificators = std::vector<Modificator*>;
    Modificators _modificators;
  };

  inline void Drawable::addToDrawPlan(DrawPlanBuildInfo& buildInfo)
  {
    if(_modificators.empty()) buildDrawActions(buildInfo);
    else
    {
      _modificators[0]->draw( buildInfo,
                              &_modificators[0] + 1,
                              _modificators.size() - 1,
                              *this);
    }
  }

  inline void Drawable::Modificator::drawNext(DrawPlanBuildInfo& buildInfo,
                                              Modificator** next,
                                              size_t modifiactorsLeft,
                                              Drawable& drawable) const
  {
    if(modifiactorsLeft != 0)
    {
      (*next)->draw(buildInfo, next + 1, modifiactorsLeft - 1, drawable);
    }
    else drawable.buildDrawActions(buildInfo);
  }
}