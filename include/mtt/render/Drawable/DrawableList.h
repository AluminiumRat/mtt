#pragma once

#include <vector>

#include <mtt/render/Drawable/Drawable.h>

namespace mtt
{
  class DrawableList : public Drawable
  {
  public:
    DrawableList() noexcept = default;
    DrawableList(const DrawableList&) noexcept = default;
    DrawableList& operator = (const DrawableList&) noexcept = default;
    virtual ~DrawableList() noexcept = default;

    inline size_t childsNumber() const noexcept;
    inline Drawable& child(size_t index) noexcept;
    inline const Drawable& child(size_t index) const noexcept;
    void addChild(Drawable& newChild);
    void removeChild(size_t index) noexcept;
    void removeChild(Drawable& removable) noexcept;

    virtual void registerAreaModificators(AreaModificatorSet& set) override;
    virtual void unregisterAreaModificators(
                                    AreaModificatorSet& set) noexcept override;

  protected:
    virtual void buildDrawActions(DrawPlanBuildInfo& buildInfo) override;

  private:
    using Childs = std::vector<Drawable*>;
    Childs _childs;

    using AreaModificators = std::vector<AreaModificatorSet*>;
    AreaModificators _areaModificators;
  };

  inline size_t DrawableList::childsNumber() const noexcept
  {
    return _childs.size();
  }

  inline Drawable& DrawableList::child(size_t index) noexcept
  {
    return *_childs[index];
  }

  inline const Drawable& DrawableList::child(size_t index) const noexcept
  {
    return *_childs[index];
  }
}