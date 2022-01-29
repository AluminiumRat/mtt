#pragma once

#include <mtt/render/SceneGraph/BoundedNode.h>

namespace mtt
{
  struct DrawPlanBuildInfo;

  class AreaModificator : public BoundedNode
  {
  protected:
    AreaModificator(size_t type);
    AreaModificator(const AreaModificator&) = delete;
    AreaModificator& operator = (const AreaModificator&) = delete;
  public:
    virtual ~AreaModificator() noexcept = default;

    inline size_t type() const noexcept;

    static size_t allocateType() noexcept;

    virtual void buildPrepareActions(DrawPlanBuildInfo& buildInfo);

  private:
    size_t _type;
  };

  inline size_t AreaModificator::type() const noexcept
  {
    return _type;
  }
}