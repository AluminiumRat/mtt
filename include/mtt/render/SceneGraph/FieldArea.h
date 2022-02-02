#pragma once

#include <optional>
#include <vector>

#include <mtt/render/SceneGraph/AreaModificatorSet.h>

namespace mtt
{
  class DrawableNode;

  class FieldArea
  {
  public:
    using Drawables = std::vector<DrawableNode*>;

  public:
    FieldArea();
    FieldArea(const FieldArea&) = delete;
    FieldArea& operator = (const FieldArea&) = delete;
    virtual ~FieldArea() noexcept;

    inline const Drawables& drawables() const noexcept;
    virtual void addDrawable(DrawableNode& node);
    virtual void removeDrawable(DrawableNode& node) noexcept;

    inline const AreaModificatorSet* modificators() const noexcept;
    virtual void addModificator(AreaModificator& modificator);
    virtual void removeModificator(AreaModificator& modificator) noexcept;

  private:
    void _attachModificators();
    void _detachModificators() noexcept;

  private:
    Drawables _drawables;
    std::optional<AreaModificatorSet> _modificators;
  };

  inline const FieldArea::Drawables& FieldArea::drawables() const noexcept
  {
    return _drawables;
  }

  inline const AreaModificatorSet* FieldArea::modificators() const noexcept
  {
    if(!_modificators.has_value()) return nullptr;
    return &_modificators.value();
  }
}