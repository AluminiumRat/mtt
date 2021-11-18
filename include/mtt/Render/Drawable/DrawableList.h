#pragma once

#include <vector>

#include <mtt/Render/Drawable/Drawable.h>

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
    inline void addChild(Drawable& newChild);
    void removeChild(size_t index) noexcept;
    void removeChild(Drawable& removable) noexcept;

  protected:
    virtual void buildDrawActions(DrawPlanBuildInfo& buildInfo) override;

  private:
    using Childs = std::vector<Drawable*>;
    Childs _childs;
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

  inline void DrawableList::addChild(Drawable& newChild)
  {
    _childs.push_back(&newChild);
  }
}